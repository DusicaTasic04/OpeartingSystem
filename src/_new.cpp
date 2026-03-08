//
// Created by os on 3/7/26.
//


// ovo se ovako ne radi ali moze za sada da posluzi :)
#include "../h/MemoryAllocator.h"

using size_t = decltype(sizeof(0));

void *operator new(size_t n) {
    return MemoryAllocator::getInstance().mem_alloc(n);
}

void *operator new[](size_t n) {
    return MemoryAllocator::getInstance().mem_alloc(n);
}

void operator delete(void *p) noexcept {
    MemoryAllocator::getInstance().mem_free(p);
}

void operator delete[](void *p) noexcept {
    MemoryAllocator::getInstance().mem_free(p);
}