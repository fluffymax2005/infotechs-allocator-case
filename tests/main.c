#include <stdio.h>
#include <stdlib.h>

#include "../headers/allocator.h"

int main() {
    Allocator* allocator = allocator_create_empty();

    void* s1 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s2 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s3 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s4 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s5 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);



    printf("s1 = %i\ns2 = %i\ns3 = %i\ns4 = %i\ns5 = %i\n", (size_type)s1, (size_type)s2, (size_type)s3, (size_type)s4, (size_type)s5);



    exit(EXIT_SUCCESS);
}
