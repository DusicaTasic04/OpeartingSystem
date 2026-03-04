//
// Created by os on 3/2/26.
//

#include "../h/MemoryAllocator.h"

MemoryAllocator& MemoryAllocator::getInstance() {
    static MemoryAllocator memoryAllocator;
    return memoryAllocator;
}

MemoryAllocator::MemoryAllocator() {
    // Align startne adrese tako da uvek bude visa u odnosu na pravu pocetnu adresu
    size_t start = (size_t)(HEAP_START_ADDR) - (size_t)(HEAP_START_ADDR) % MEM_BLOCK_SIZE + MEM_BLOCK_SIZE;
    // Slicno za end, ali se ne dodaje MEM_BLOCK_SIZE da ne izadjemo iz opsega:
    size_t end = (size_t)(HEAP_END_ADDR) - (size_t)(HEAP_END_ADDR) % MEM_BLOCK_SIZE;

    head = (FreeMemoryBlock*)(start); // Glava se nalazi na pocetku
    head->next = nullptr;
    head->size = (end - start) / MEM_BLOCK_SIZE;
}

size_t MemoryAllocator::toByte(size_t size) {
    return size * MEM_BLOCK_SIZE;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size == 0 || !head) return nullptr;

    size_t byteSize = toByte(size); // Velicina u bajtovima
    FreeMemoryBlock *tmp = head;
    FreeMemoryBlock *prev = nullptr;

    while (tmp && tmp->size <= byteSize) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (!tmp) return nullptr; // Prosli smo kroz celu listu i nema prostora

    size_t remaining = tmp->size - byteSize;
    if (remaining >= sizeof(FreeMemoryBlock) + byteSize) {

    }
}

int MemoryAllocator::mem_free(void *) {
    return 0;
}

size_t MemoryAllocator::mem_get_free_space() {
    size_t sum = 0;
    FreeMemoryBlock* tmp = head;
    while(tmp) {
        sum += tmp->size;
        tmp = tmp->next;
    }
    return sum;
}

size_t MemoryAllocator::mem_get_largest_free_block() {
    size_t max = 0;
    FreeMemoryBlock* tmp = head;
    while(tmp) {
        if (tmp->size > max) {
            max = tmp->size;
        }
        tmp = tmp->next;
    }
    return max;
}


