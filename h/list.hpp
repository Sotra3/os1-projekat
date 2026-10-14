//
// Created by os on 25. 8. 2026..
//

#ifndef BASE_PROJECT_LIST_H
#define BASE_PROJECT_LIST_H
#include "syscall_c.h"
#include "../h/MemoryAllocator.h"

class TCB;

class List {
public:

    typedef struct elem{
        TCB* data;
        elem* next;
    }Elem;

    Elem* head;

    List():head(0) {};

    void addItem(TCB* data) {
        Elem* newElem = (Elem*)MemoryAllocator::mem_alloc(sizeof(Elem));
        newElem->data = data;
        newElem->next = 0;
        Elem* curr=head;
        if (head) {
            while (curr->next) {curr=curr->next;}
            curr->next = newElem;
        }
        else {
            head = newElem;
        }
    }

    TCB* popItem() {
        if (!head) return 0;
        Elem* curr = head;
        TCB* tmpTCB=head->data;
        head = head->next;
        MemoryAllocator::mem_free(curr);
        return tmpTCB;
    }

    TCB* peekItem() {
        if (!head) return nullptr;
        return head->data;
    }
};

#endif //BASE_PROJECT_LIST_H
