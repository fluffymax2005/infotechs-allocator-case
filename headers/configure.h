#ifndef CONFIGURATION_H
    #define CONFIGURATION_H

    #include <stddef.h>
    
    #define PROCESSOR_ARCHITECTURE (8 * sizeof(void*))
    #define MALLOC_ALIGNMENT alignof(max_align_t)

#endif