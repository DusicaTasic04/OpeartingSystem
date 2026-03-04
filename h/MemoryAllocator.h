#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_H

#include "../lib/hw.h"

struct FreeMemoryBlock {
    FreeMemoryBlock* next;
    size_t size;
};

class MemoryAllocator {
public:
    static MemoryAllocator& getInstance(); // Singleton

    void* mem_alloc (size_t size);
    int mem_free (void*);
    size_t mem_get_free_space();
    size_t mem_get_largest_free_block();

private:
    FreeMemoryBlock* head = nullptr;
    MemoryAllocator();
    size_t toByte(size_t size);
};


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
