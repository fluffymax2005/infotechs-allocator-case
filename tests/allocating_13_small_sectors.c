#include <stdio.h>
#include <stdlib.h>

#include "../src/include/allocator.h"

#define ARR_SIZE 13

int main() {
    Allocator* allocator = allocator_create_empty();

    void* arr[ARR_SIZE];

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

    arr[0] = s1;
    arr[1] = s2;
    arr[2] = s3;
    arr[3] = s4;
    arr[4] = s5;
    arr[5] = s6;
    arr[6] = s7;
    arr[7] = s8;
    arr[8] = s9;
    arr[9] = s10;
    arr[10] = s11;
    arr[11] = s12;
    arr[12] = s13;

    for (sbyte i = 0; i < ARR_SIZE; ++i) {
        printf("Small[%i] sector's address = %p\n", i, arr[i]);
    }

    exit(EXIT_SUCCESS);
}
