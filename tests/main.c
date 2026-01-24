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
    void* s6 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s7 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s8 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s9 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s10 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s11 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s12 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);

    void* s13 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);

    void* b1 = allocator_alloc(allocator, BIG_BLOCK_SIZE);

    printf("s1 = %i\ns2 = %i\ns3 = %i\ns4 = %i\ns5 = %i\ns12 = %i\ns13 = %i\nb1 = %i\n", 
        (size_type)s1, (size_type)s2, (size_type)s3, (size_type)s4, (size_type)s5, (size_type)s12, (size_type)s13, (size_type)b1);

    for (sbyte i = 11; i >= 0 ; --i)
        printf("%c", '0' + IS_SECTOR_OCCUPIED(allocator->_first->sector, i));
    printf("\n");


    exit(EXIT_SUCCESS);
}
