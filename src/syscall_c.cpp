//
// Created by os on 3/8/26.
//

#include "../h/syscall_c.hpp"

void* mem_alloc (size_t size) {
    size *= MEM_BLOCK_SIZE; // Zato sto moj mem_alloc vec ubacuje kao broj bajtova
    __asm__ volatile("mv a1, %0" :: "r"(size)); // prosledim parametre

    __asm__ volatile ("mv a0, %0" :: "r"(0x01)); // u a0 premestim kod
    __asm__ volatile ("ecall"); // skok u privilegovani rezim

    // povratak iz privilegovanog rezima:
    void* returnValue;
    __asm__ volatile ("mv %0, a0":"=r"(returnValue));
    return returnValue;
}

int mem_free(void* ptr) {
    __asm__ volatile ("mv a1, %0" :: "r"(ptr));

    __asm__ volatile ("mv a0, %0" :: "r"(0x02)); // u a0 premestim kod
    __asm__ volatile ("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0":"=r"(returnValue));
    return returnValue;
}

size_t mem_get_free_space() {
    __asm__ volatile ("mv a0, %0" :: "r"(0x03));
    __asm__ volatile ("ecall");

    size_t returnValue;
    __asm__ volatile ("mv %0, a0":"=r"(returnValue));
    return returnValue;
}

size_t mem_get_largest_free_block() {
    __asm__ volatile ("mv a0, %0" :: "r"(0x04));
    __asm__ volatile ("ecall");

    size_t returnValue;
    __asm__ volatile ("mv %0, a0":"=r"(returnValue));
    return returnValue;
}
