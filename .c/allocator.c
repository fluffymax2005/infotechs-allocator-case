#include "../headers/allocator.h"
#include <stdio.h>
#include <stdlib.h>

Allocator* allocator_create_empty() {
    return (Allocator*)calloc(1, sizeof(Allocator));
}
    
Allocator* allocator_create_with_pool(const size_type bytes) {
    Allocator* allocator = (Allocator*)calloc(1, sizeof(Allocator));

    if (allocator == NULL)
        return NULL;

    // Block size rules:
    // 1. If bytes < DEFAULT_ALLOCATOR_BLOCK_SIZE then (DEFAULT_ALLOCATOR_BLOCK_SIZE + sizeof(Header)) is used
    // 2. Else sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE * <number_of_blocks> is used
    const size_type min_blocks_count = bytes <= DEFAULT_ALLOCATOR_BLOCK_SIZE ? 1 : bytes / DEFAULT_ALLOCATOR_BLOCK_SIZE + 1;
    const size_type block_size = min_blocks_count * (sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);

    void* heap_block = malloc(block_size);
    if (heap_block == NULL) {
        free(allocator);
        return NULL;
    }

    // Initialize allocator fields
    Header* heap_header = (Header*)heap_block;
    heap_header->block = EMPTY;
    heap_header->next = NULL;
    allocator->_first = heap_block;
    //allocator->_last = min_blocks_count == 1 ? heap_block : (char*)heap_block + (min_blocks_count - 1) * (sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);

    return allocator;
}


void* allocator_alloc(Allocator* allocator, const size_type bytes) {
    if (!allocator || !allocator->_first /*|| !allocator->_last*/ || !(bytes == SMALL_BLOCK_SIZE || bytes == BIG_BLOCK_SIZE))
        return NULL;
    switch (bytes) {
        case SMALL_BLOCK_SIZE: return __alloc_small_block(allocator);
        case BIG_BLOCK_SIZE: return __alloc_big_block(allocator);
        default: return NULL;
    }

}

void* __alloc_small_block(Allocator* allocator) {
    if (allocator == NULL)
        return NULL;

    // Searching for first free sector
    Header* ptr = allocator->_first;
    while (1) {
        if (ptr->block == EMPTY || ptr->block == SMALL) {
            char available_sector = IS_BLOCK_OCCUPIED(ptr->sector); // number of first free sector
            if (available_sector != BLOCK_OCCUPIED) {
                // Update information about sectors
                ptr->block = SMALL;
                SET_SECTOR_OCCUPIED(ptr->sector, available_sector);

                return (char*)ptr + sizeof(Header) + 15 * available_sector;
            }            
        }

        // If next is NULL then ptr must not change
        if (ptr->next == NULL)
            break;
        ptr = ptr->next;
    }

    // Searching failed - there are no free space. Need to expand
    void* new_space = calloc(1, sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);
    if (new_space == NULL)
        return NULL;

    // Fill header information
    Header* new_header = (Header*)new_space;
    new_header->block = SMALL;
    SET_SECTOR_OCCUPIED(new_header->sector, 0);
    ptr->next = new_header;

    return (char*)new_space + sizeof(Header);
}

static void* __alloc_big_block(Allocator* allocator) {
    if (allocator == NULL)
        return NULL;

    // Searching for first free sector
    Header* ptr = allocator->_first;
    while (1) {
        if (ptr->block == EMPTY && IS_BLOCK_OCCUPIED(ptr->sector) != BLOCK_OCCUPIED) {
            return (char*)ptr + sizeof(Header);
        }

        if (ptr->next == NULL)
            break;
        ptr = ptr->next;
    }

    // Searching failed - there no free space. Need to expand
    void* new_space = calloc(1, sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);
    if (new_space == NULL)
        return NULL;

    // Fill header information
    Header* new_header = (Header*)new_space;
    new_header->block = BIG;
    SET_SECTOR_OCCUPIED(new_header->sector, 0);
    ptr->next = new_header;

    return (char*)new_space + sizeof(Header);
}
