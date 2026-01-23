#include "../headers/allocator.h"
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
    allocator->_first = heap_block;
    allocator->_last = min_blocks_count == 1 ? heap_block : (char*)heap_block + (min_blocks_count - 1) * (sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);

    return allocator;
}


void* allocator_alloc(Allocator* allocator, const size_type bytes) {
    if (!allocator || !allocator->_first || !allocator->_last || !(bytes == SMALL_BLOCK_SIZE || bytes == BIG_BLOCK_SIZE))
        return NULL;

    Header* ptr = allocator->_first;
    Header* free = NULL;



    // Searching for first free sector
    while (ptr <= allocator->_last) {
        if (bytes == BIG_BLOCK_SIZE && ptr->block == EMPTY) { // for 180 bytes
            free = ptr;
            break;
        } else if (bytes == SMALL_BLOCK_SIZE && (ptr->block == EMPTY || ptr->block == SMALL)) { // for 15 bytes
            char sector = IS_BLOCK_OCCUPIED(ptr->sector); // number of first free sector
            if (sector == -1)
                continue; // not found
            free = ptr;
        }
    }
    return (char*)ptr + sizeof(Header);
}

void* __alloc_small_block(Allocator* allocator, Header* first, Header* last) {
    if (!first || !last || (size_type)first < (size_type)last)
        return NULL;

    // Searching for first free sector
    while (first <= last) {
        if (first->block == EMPTY || first->block == SMALL) {
            char available_sector = IS_BLOCK_OCCUPIED(first->sector); // number of first free sector
            if (available_sector == -1)
                continue; // not found
            return (char*)first + sizeof(Header) + 15 * available_sector;
        }
        first = (Header*)((char*)first + sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);
    }

    // Searching failed - there are no free space. Need to expand
    void* new_space = calloc(1, sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);
    if (new_space == NULL)
        return NULL;

    // Fill header information
    Header* new_header = (Header*)new_space;
    new_header->block = SMALL;
    SET_SECTOR_OCCUPIED(new_header->sector, 0);

    return (char*)new_space + sizeof(Header);
}
