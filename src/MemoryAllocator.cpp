//
// Created by os on 3/2/26.
//

#include "../h/MemoryAllocator.h"

MemoryAllocator* MemoryAllocator::getInstance() {
    static MemoryAllocator memoryAllocator;
    return &memoryAllocator;
}

MemoryAllocator::MemoryAllocator() {
    // Align startne adrese na prvi >= memorijski blok
    const size_t start = ((size_t)HEAP_START_ADDR + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;
    // Slicno za end:
    const size_t end = (size_t)(HEAP_END_ADDR) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;

    head = (FreeMemoryBlock*)(start); // Glava se nalazi na pocetku
    head->next = nullptr;

    const size_t headerSize = (sizeof(FreeMemoryBlock) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;
    head->size = (end - start) - headerSize; // Cuvam slobodan prostor u bajtovima, ukljucuje zaglavlje strukture
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size == 0 || !head) return nullptr;

    size = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE; // Poravnato
    const size_t headerSize = (sizeof(FreeMemoryBlock) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;

    const size_t totalNeeded = size + headerSize;

    FreeMemoryBlock *tmp = head;
    FreeMemoryBlock *prev = nullptr;

    while (tmp && tmp->size < totalNeeded) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (!tmp) return nullptr; // Prosli smo kroz celu listu i nema prostora

    size_t remaining = tmp->size - totalNeeded;

    if (remaining >= headerSize + MEM_BLOCK_SIZE) { // Da li u preostalom delu ima mesta da alociram zaglavlje strukture?
		auto* newBlock = (FreeMemoryBlock*)((char*)tmp + totalNeeded);
        newBlock->size = remaining - headerSize;
        newBlock->next = tmp->next;

        if (prev)
            prev->next = newBlock;
        else
            head = newBlock;

        tmp->size = totalNeeded;
        return (void*)((char*)tmp + headerSize);
    }
	// else
    if (prev)
        prev->next = tmp->next;
    else
        head = tmp->next;

    tmp->size = totalNeeded;
    return (void*)((char*)tmp + headerSize);
}

int MemoryAllocator::mem_free(void *ptr) {
    if (!ptr) return -1; // Ne mogu da oslobodim nullptr

    const size_t headerSize = (sizeof(FreeMemoryBlock) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;
    auto* toFree = (FreeMemoryBlock*)((char*)ptr - headerSize);

    FreeMemoryBlock* prev = nullptr;
    FreeMemoryBlock* tmp = head;

    //int i = 0;
    while (tmp && tmp < toFree) {
        prev = tmp;
        tmp = tmp->next;
        //i++;
    }

    //printInt(i); printString("\n");

    toFree->next = tmp;
    if (prev)
        prev->next = toFree;
    else
        head = toFree;

    if (prev) {
        if (coalesce(prev)) // Spoji prev i toFree ako su susedni elementi
            toFree = prev;
    }
	if (tmp) coalesce(toFree); // Spoji toFree i tmp ako su susedni elementi

    return 0; // Ocekivano ponasanje programa
}

int MemoryAllocator::coalesce(FreeMemoryBlock* ptr) {
    if (!ptr) return 0;

    const size_t headerSize = (sizeof(FreeMemoryBlock) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE * MEM_BLOCK_SIZE;
	if (ptr->next && (char*)ptr + headerSize + ptr->size == (char*)ptr->next) {

        ptr->size += headerSize + ptr->next->size;
        ptr->next = ptr->next->next;
	    return 1;
    }
    return 0;
}

size_t MemoryAllocator::mem_get_free_space() const {
    size_t sum = 0;
    FreeMemoryBlock* tmp = head;
    while(tmp) {
        sum += tmp->size;
        tmp = tmp->next;
    }
    return sum;
}

size_t MemoryAllocator::mem_get_largest_free_block() const {
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
