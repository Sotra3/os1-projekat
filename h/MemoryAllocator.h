//
// Created by os on 8/19/26.
//

#ifndef BASE_PROJECT_MEMORYALLOCATOR_H
#define BASE_PROJECT_MEMORYALLOCATOR_H

#include "../lib/hw.h"

typedef struct memmeta {
    size_t size;
    void* next;
}MemMeta;

class MemoryAllocator {
    static MemMeta* firstFree;
    static MemMeta* firstUsed;
public:
    static void* mem_alloc (size_t size);
    static int mem_free (void* ptr);
};

#endif //BASE_PROJECT_MEMORYALLOCATOR_H
