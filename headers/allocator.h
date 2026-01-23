#ifndef MY_ALLOCATOR_H
    #define MY_ALLOCATOR_H

    #include "configure.h"
    #include <stdint.h>

    #define SMALL_BLOCK_SIZE 15
    #define BIG_BLOCK_SIZE 180
    #define SMALL_SECTORS_IN_BIG ((BIG_BLOCK_SIZE) / (BIG_BLOCK_SIZE))

    #define DEFAULT_ALLOCATOR_BLOCK_SIZE BIG_BLOCK_SIZE
    
    #define IS_SECTOR_OCCUPIED(state_mask, n) (((state_mask) >> (n)) & 1)
    #define SET_SECTOR_OCCUPIED(state_mask, n) ((state_mask) |= (1 << (n)))
    #define CLEAR_SECTOR_OCCUPIED(state_mask, n) ((state_mask) &= ~(1 << (n)))
    #define IS_BLOCK_OCCUPIED(state_mask) \
        ((!IS_SECTOR_OCCUPIED(state_mask, 0)) ? (0) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 1)) ? (1) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 2)) ? (2) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 3)) ? (3) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 4)) ? (4) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 5)) ? (5) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 6)) ? (6) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 7)) ? (7) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 8)) ? (8) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 9)) ? (9) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 10)) ? (10) : \
        (!IS_SECTOR_OCCUPIED(state_mask, 11)) ? (11) : -1)
        
    
    typedef unsigned char byte; // 8 bit
    typedef unsigned short word; // 16 bit
    typedef unsigned int dword; // 32 bit
    typedef unsigned long long qword; // 64 bit

    typedef uintptr_t size_type; // size of ptr

    typedef enum : byte { 
        EMPTY, SMALL, BIG
    } block_type;

    typedef struct {
        word sector:11;
        block_type block;
    } Header;

    typedef struct {
        Header* _first;
        Header* _last;
    } Allocator;

    Allocator* allocator_create_empty();
    Allocator* allocator_create_with_pool(const size_type bytes);

    void* allocator_alloc(Allocator* allocator, size_type bytes);
    void allocator_free(Allocator* allocator);

    // Help functions
    static void* __alloc_small_block(Allocator* allocator, Header* first, Header* last);
    static void* __alloc_big_block(Allocator* allocator, Header* first, Header* last);
#endif