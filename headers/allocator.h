#ifndef MY_ALLOCATOR_H
    #define MY_ALLOCATOR_H

    #include <stdint.h>
    #include <stdbool.h>

    #define SMALL_BLOCK_SIZE 15
    #define BIG_BLOCK_SIZE 180
    #define SMALL_SECTORS_IN_BIG ((BIG_BLOCK_SIZE) / (BIG_BLOCK_SIZE))
    #define BLOCK_OCCUPIED -1

    #define DEFAULT_ALLOCATOR_BLOCK_SIZE BIG_BLOCK_SIZE
    
    #define IS_SECTOR_OCCUPIED(state_mask, n) (((state_mask) >> (n)) & 1)
    #define SET_SECTOR_OCCUPIED(state_mask, n) ((state_mask) |= (1 << (n)))
    #define CLEAR_SECTOR_OCCUPIED(state_mask, n) ((state_mask) &= ~(1 << (n)))

    typedef signed char sbyte; // 8 bit signed
    typedef unsigned char byte; // 8 bit
    typedef unsigned short word; // 16 bit
    typedef unsigned int dword; // 32 bit
    typedef unsigned long long qword; // 64 bit

    typedef uintptr_t size_type; // size of ptr

    typedef enum : byte { 
        EMPTY, SMALL, BIG
    } block_type;

    typedef struct Header {
        struct Header* next;
        word sector:11;
        block_type block;
    } Header;

    typedef struct Allocator {
        Header* _first;
    } Allocator;

    Allocator* allocator_create_empty();
    Allocator* allocator_create_with_pool(const size_type bytes);

    void* allocator_alloc(Allocator* allocator, const size_type bytes);
    void allocator_free(Allocator* allocator, void* ptr);

    // Help functions
    static void* __alloc_small_sector(Allocator* allocator);
    static void* __alloc_big_sector(Allocator* allocator);

    static void __free_small_sector(void* ptr, Header* header);
    static void __free_big_sector(void* ptr, Header* header);

    static inline sbyte __find_free_sector(const word mask);
    static inline bool __is_block_free(const word mask);
    static inline void __reset_block_occupied(Header* header);
#endif