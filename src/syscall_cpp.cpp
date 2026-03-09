//
// Created by os on 3/8/26.
//

#include "../h/syscall_cpp.hpp"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* ptr) {
    mem_free(ptr);
}
