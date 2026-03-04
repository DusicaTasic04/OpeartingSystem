//
// Created by os on 3/2/26.
//

#include "../h/MemoryAllocator.h"

MemoryAllocator& MemoryAllocator::getInstance() {
    static MemoryAllocator memoryAllocator;
    return memoryAllocator;
}

MemoryAllocator::MemoryAllocator() {
    // Align startne adrese na prvi >= memorijski blok
    size_t start = ((size_t)HEAP_START_ADDR + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;
    // Slicno za end:
    size_t end = (size_t)(HEAP_END_ADDR) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;

    head = (FreeMemoryBlock*)(start); // Glava se nalazi na pocetku
    head->next = nullptr;
    head->size = (end - start); // Cuvam slobodan prostor u bajtovima
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size == 0 || !head) return nullptr;

    FreeMemoryBlock *tmp = head;
    FreeMemoryBlock *prev = nullptr;

    while (tmp && tmp->size < size) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (!tmp) return nullptr; // Prosli smo kroz celu listu i nema prostora

    size_t remaining = tmp->size - size;
    if (remaining >= sizeof(FreeMemoryBlock)) { // Da li u preostalom delu ima mesta da alociram zaglavlje strukture?

    }
    if (prev)
    	prev->next = tmp->next;
	else
    	head = tmp->next;

    return (void*)((char*)tmp + sizeof(FreeMemoryBlock));
}

int MemoryAllocator::mem_free(void *ptr) {
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


