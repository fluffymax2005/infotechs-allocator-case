#include <stdio.h>
#include <stdlib.h>

#include "../headers/allocator.h"

#define ARR_SIZE 4

int main() {
    Allocator* allocator = allocator_create_empty();

    void* arr[ARR_SIZE];

    void* s1 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s2 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);
    void* s3 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);

    void* b1 = allocator_alloc(allocator, SMALL_BLOCK_SIZE);

    arr[0] = s1;
    arr[1] = s2;
    arr[2] = s3;
    arr[3] = b1;

    for (sbyte i = 0; i < ARR_SIZE - 1; ++i) {
        printf("Small[%i] sector's address = %p\n", i, arr[i]);
    }

    printf("Big[%i] sector's address = %p\n", 0, arr[ARR_SIZE - 1]);

    exit(EXIT_SUCCESS);
}
