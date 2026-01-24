#include "../headers/allocator.h"
#include <stdlib.h>
#include <stdbool.h>

Allocator* allocator_create_empty() {
    return (Allocator*)calloc(1, sizeof(Allocator));
}
    
Allocator* allocator_create_with_pool(const size_type bytes) {
    if (bytes != SMALL_BLOCK_SIZE && bytes != BIG_BLOCK_SIZE)
        return NULL;
    
    Allocator* allocator = (Allocator*)calloc(1, sizeof(Allocator));
    if (allocator == NULL)
        return NULL;

    void* heap_block = malloc(sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE);
    if (heap_block == NULL) {
        free(allocator);
        return NULL;
    }

    // Initialize allocator fields
    Header* heap_header = (Header*)heap_block;
    heap_header->block = EMPTY;
    heap_header->next = NULL;
    heap_header->sector = 0;

    allocator->_first = heap_block;
    return allocator;
}

void allocator_destroy_allocator(Allocator* allocator) {
    if (allocator == NULL || allocator->_first == NULL)
        return;
    Header* next = allocator->_first;
    while (next) {
        Header* cur = next;
        next = cur->next;
        free(cur);        
    }
    free(allocator);
}

void* allocator_alloc(Allocator* allocator, const size_type bytes) {
    if (!allocator || !(bytes == SMALL_BLOCK_SIZE || bytes == BIG_BLOCK_SIZE))
        return NULL;
    switch (bytes) {
        case SMALL_BLOCK_SIZE: return __alloc_small_sector(allocator);
        case BIG_BLOCK_SIZE: return __alloc_big_sector(allocator);
        default: return NULL;
    }

}

void* __alloc_small_sector(Allocator* allocator) {
    if (allocator == NULL)
        return NULL;

    // Searching for first free sector
    Header* ptr = allocator->_first;
    while (ptr) {
        if (ptr->block == EMPTY || ptr->block == SMALL) {
            const sbyte available_sector = __find_free_sector(ptr->sector); // number of first free sector
            if (available_sector != BLOCK_OCCUPIED) {
                // Update information about sectors
                ptr->block = SMALL;
                SET_SECTOR_OCCUPIED(ptr->sector, available_sector);

                return (char*)ptr + sizeof(Header) + SMALL_BLOCK_SIZE * available_sector;
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

    if (ptr)
        ptr->next = new_header; // concat new allocated memory to existing
    else
        allocator->_first = new_header; // mark new allocated memory as first allocation

    return (char*)new_space + sizeof(Header);
}

void* __alloc_big_sector(Allocator* allocator) {
    if (allocator == NULL)
        return NULL;

    // Searching for first free sector
    Header* ptr = allocator->_first;
    while (ptr) {
        if (ptr->block == EMPTY) {
            __set_block_occupied(ptr);
            ptr->block = BIG;
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
    __set_block_occupied(new_header);
    
    if (ptr)
        ptr->next = new_header; // concat new allocated memory to existing
    else
        allocator->_first = new_header; // mark new allocated memory as first allocation

    return (char*)new_space + sizeof(Header);
}

void allocator_free(Allocator* allocator, void* ptr) {
    if (allocator == NULL || ptr == NULL)
        return;

    // User can free only that was given by allocator_alloc. Otherwise, nothing is done
    Header* block_header = allocator->_first;
    while (block_header) {
        // ptr is in range [Header + sizeof(Header); Header + sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE - SMALL_BLOCK_SIZE]
        if ((size_type)ptr >= (size_type)block_header + sizeof(Header) &&
            (size_type)ptr <= (size_type)block_header + sizeof(Header) + DEFAULT_ALLOCATOR_BLOCK_SIZE - SMALL_BLOCK_SIZE) {
                switch (block_header->block) {
                    case SMALL: __free_small_sector(ptr, block_header); break;
                    case BIG: __free_big_sector(ptr, block_header); break;
                }
                return;
        }
        block_header = block_header->next;
    }
}

void __free_small_sector(void* ptr, Header* header) {
    // Ptr check. If (ptr - (header + sizeof(Header))) % SMALL_BLOCK_SIZE != 0 then bad address is given
    if (ptr == NULL || header == NULL || ((size_type)ptr - ((size_type)header + sizeof(Header))) % SMALL_BLOCK_SIZE)
        return;

    // Free sector
    const byte sector_occupied = ((size_type)ptr - (size_type)header - sizeof(Header)) / SMALL_BLOCK_SIZE;
    CLEAR_SECTOR_OCCUPIED(header->sector, sector_occupied);

    // If all sectors of block are free then mark block as free
    const word block_mask = header->sector;
    if (__is_block_free(block_mask))
        header->block = EMPTY;
}

void __free_big_sector(void* ptr, Header* header) {
    // Ptr check. If (ptr - (header + sizeof(Header))) != 0 then bad address is given
    if (ptr == NULL || header == NULL || (size_type)ptr - ((size_type)header + sizeof(Header)))
        return;

    // Free sector (block) and mark as empty;
    __reset_block_occupied(header);
}

sbyte __find_free_sector(const word mask) {
    for (byte i = 0; i < 12; ++i)
        if (!IS_SECTOR_OCCUPIED(mask, i)) return i;
    return BLOCK_OCCUPIED;
}

bool __is_block_free(const word mask) {
    for (byte i = 0; i < 12; ++i)
        if (IS_SECTOR_OCCUPIED(mask, i)) return false;
    return true;
}

void __set_block_occupied(Header* header) {
    if (header == NULL)
        return;
    for (byte i = 0; i < 12; ++i)
        SET_SECTOR_OCCUPIED(header->sector, i);
}

void __reset_block_occupied(Header* header) {
    if (header == NULL)
        return;
    for (byte i = 0; i < 12; ++i)
        CLEAR_SECTOR_OCCUPIED(header->sector, i);
    header->block = EMPTY;
}