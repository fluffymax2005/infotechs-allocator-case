#include "../headers/allocator.h"

#include <cstdlib>

Allocator* allocator_init() {
    Allocator* allocator = (Allocator*)malloc(sizeof(Allocator));

    if (allocator == NULL)
        return NULL;

    #ifdef DEFAULT_ALLOCATOR_BLOCK_SIZE
        size_type block_size = DEFAULT_ALLOCATOR_BLOCK_SIZE;
    #else
        size_type block_size = 0; 
    #endif

    if (sizeof(Header) >= block_size)
        return allocator;

    void* block = malloc(block_size);
    
    // TODO: make algorithm to manage memory

    if (block != NULL) {
        Header* free_blocks = (Header*)malloc(sizeof(Header));
        if (free_blocks == NULL) {
            free(block);
            return allocator;
        }

        allocator->start = block;
        allocator->end = (bool*)block + block_size;

        Header first_free_block;
        first_free_block.available_bytes = block_size - sizeof(Header);

        free_blocks[0] = first_free_block;
        allocator->max_block = &first_free_block;
    }

    return allocator;
}

//void* allocator_alloc()