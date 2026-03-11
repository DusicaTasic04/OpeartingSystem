//
// Created by os on 3/7/26.
//

#ifndef PROJECT_BASE_V1_2_LIST_H
#define PROJECT_BASE_V1_2_LIST_H

#include "syscall_cpp.hpp"

template <typename T>
class List {
public:
    List() : size(0), head(nullptr), tail(nullptr) {} // Konstruktor

    void addFirst(T *data) {
        size++;
        Element *elem = new Element(data, head);
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data) {
        size++;
        Element *e = new Element(data, nullptr);
        if (tail) {
            tail->next = e;
            tail = e;
        }
        else {
            head = e;
            tail = e;
        }
    }

    T* removeFirst() {
        size--;
        if (!head) return nullptr;

        Element *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        return ret;
    }

    T* removeLast() {
        size--;
        if (!head) { return 0; }

        Element *prev = 0;
        for (Element *curr = head; curr && curr != tail; curr = curr->next) {
            prev = curr;
        }

        Element *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        return ret;

    }

    T* getFirst() {
        return head ? head->data : nullptr;
    }

    T* getLast() {
        return tail ? tail->data : nullptr;
    }

    int getSize() const {
        return size;
    }

private:
    struct Element
    {
        T *data;
        Element *next;

        Element(T *data, Element *next) : data(data), next(next) {}
    };

    int size;
    Element *head, *tail;
};


#endif //PROJECT_BASE_V1_2_LIST_H