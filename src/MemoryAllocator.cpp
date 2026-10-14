//
// Created by os on 8/19/26.
//

#include "../h/MemoryAllocator.h"

MemMeta* MemoryAllocator::firstFree = nullptr;
MemMeta* MemoryAllocator::firstUsed = nullptr;

void *MemoryAllocator::mem_alloc(size_t size) {
    MemMeta* curr;
    MemMeta* curr2;
    MemMeta* newUsed = nullptr;
    MemMeta* newFree = nullptr;
    size_t actualSize;


    // inicijalizacija pri prvom trazenju memorije posle boot-a
    if (firstFree == nullptr) {
        firstFree = (MemMeta*)HEAP_START_ADDR;
        firstFree->size = ((((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR) - sizeof(MemMeta)) / MEM_BLOCK_SIZE) * MEM_BLOCK_SIZE;
        firstFree->next = nullptr;
    }

    curr = firstFree;
    actualSize = size + (size % MEM_BLOCK_SIZE ? 1 : 0) * (MEM_BLOCK_SIZE - size % MEM_BLOCK_SIZE);

    while (curr != nullptr) {
        if (curr->size > (actualSize + sizeof(MemMeta))) break; //nasli smo dovoljno veliki blok
        curr = (MemMeta*)curr->next;
    }

    if (curr == nullptr) return nullptr;


    for(curr2 = firstFree; curr != firstFree && curr2->next != curr && curr2->next != nullptr; curr2 = (MemMeta*)curr2->next);
    newFree = (MemMeta*)((size_t)curr + actualSize + sizeof(MemMeta));
    if (curr != firstFree) curr2->next = newFree;
    else firstFree = newFree;
    newFree->size = curr->size - actualSize - sizeof(MemMeta);
    newFree->next = curr->next;

    if (firstUsed == nullptr) {
        firstUsed = curr;
        firstUsed->size = actualSize;
        firstUsed->next = nullptr;
        newUsed = firstUsed;
    }

    else {
        for (curr2 = firstUsed; curr2->next != nullptr; curr2 = (MemMeta*)curr2->next);
        newUsed = curr;
        curr2->next = newUsed;
        newUsed->size = actualSize;
        newUsed->next = nullptr;
    }

    return (void*)((size_t)newUsed + sizeof(MemMeta));
}

int MemoryAllocator::mem_free (void* ptr) {
    MemMeta* curr;
    void* tmp;
    MemMeta* newFree;

    //rastuci poredak

    if ((void*)((size_t)ptr - sizeof(MemMeta)) == firstUsed) {
        firstUsed = (MemMeta*)firstUsed->next;
    }
    else {
        for (curr = firstUsed; curr->next != (void*)((size_t)ptr - sizeof(MemMeta)) && curr->next != nullptr; curr = (MemMeta*)curr->next);
        if (curr->next == nullptr) return -1;
        curr->next = ((MemMeta*)((size_t)ptr - sizeof(MemMeta)))->next;
    } // vrsi prelancavanje USED liste

    newFree = (MemMeta*)((size_t)ptr - sizeof(MemMeta));

    if ((MemMeta*)((size_t)ptr - sizeof(MemMeta)) < firstFree) {
        newFree->next = firstFree;
        firstFree = newFree;
    }

    else {
        for (curr = firstFree; (size_t)curr->next < ((size_t)ptr - sizeof(MemMeta)) && curr->next != nullptr; curr = (MemMeta*)curr->next);
        tmp = curr->next;
        curr->next = newFree;
        newFree->next = tmp;

    }

    for (curr = firstFree; curr->next != nullptr; ){
        if ((size_t)curr + curr->size + sizeof(MemMeta) == (size_t)curr->next){
            curr->size = curr->size + ((MemMeta*)curr->next)->size + sizeof(MemMeta);
            curr->next = ((MemMeta*)curr->next)->next;
        }
        else {
            curr = (MemMeta*)curr->next;
        }
    }

    return 0;
}