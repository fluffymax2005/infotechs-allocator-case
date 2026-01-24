#include <stdio.h>
#include <stdlib.h>

#include "../headers/allocator.h"

int main() {
    Allocator* allocator = allocator_create_with_pool(SMALL_BLOCK_SIZE);

    void* ptr1 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* ptr2 = allocator_alloc(allocator, BIG_BLOCK_SIZE);


    printf("Allocator=%i\nptr1 = %i\nptr2 = %i\nPointer difference (without header) = %i\n", 
        ptr1, ptr2, allocator, 
        (size_type)ptr2 - (size_type)ptr1);
}
