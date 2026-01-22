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

    void* block = malloc(block_size);
    
    // TODO: make algorithm to manage memory

    if (block != NULL) {
        //void* free_block = malloc();

        allocator->start = block;
        allocator->end = (bool*)block + block_size;

        //while (*block++)
    }

    return allocator;
}

//void* allocator_alloc()