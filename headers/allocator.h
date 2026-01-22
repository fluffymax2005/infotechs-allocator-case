#ifndef MY_ALLOCATOR_H
    #define MY_ALLOCATOR_H

    #include "configure.h"
    #include <cstdint>

    #define SMALL_BLOCK_SIZE 15
    #define BIG_BLOCK_SIZE 180

    #define DEFAULT_ALLOCATOR_BLOCK_SIZE BIG_BLOCK_SIZE

    typedef unsigned char byte; // 8 bit
    typedef unsigned short int word; // 16 bit
    typedef unsigned int dword; // 32 bit
    typedef unsigned long long qword; // 64 bit

    typedef uintptr_t size_type; // size of ptr

    typedef struct {
        size_type available_bytes = 0;    
    } Header;

    /*typedef struct {
        typedef byte flag_type;

        size_type size = 0;
        void* ptr = NULL;
        flag_type isFree:1 = 1;
    } Block;*/

    typedef struct {
        void* start = NULL;
        void* end = NULL;
        Header* free_blocks = NULL;
        Header* max_block = NULL;

    } Allocator;

    Allocator* allocator_init();
    void* allocator_alloc(Allocator* allocator, size_type bytes);
    void allocator_dealloc(Allocator* allocator);
    


    //static Allocator alloc;

    

#endif