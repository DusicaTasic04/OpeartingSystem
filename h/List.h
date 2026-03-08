//
// Created by os on 3/7/26.
//

#ifndef PROJECT_BASE_V1_2_LIST_H
#define PROJECT_BASE_V1_2_LIST_H

template <typename T>
class List {
public:
    List() : size(0), head(nullptr), tail(nullptr) {} // Konstruktor

    void addFirst(T *data) {
        size++;
        if (head) {
            Element *e = new Element(data, head);
            head = e;
        }
        else {
            Element *e = new Element(data, nullptr);
            head = e;
            tail = e;
        }
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

        const Element *e = head;
        T* data = head->data;
        head = head->next;
        if (!head) tail = nullptr;

        delete e;
        return data;
    }

    T* removeLast() {
        size--;
        if (!head) return nullptr;

        T* data = tail->data;
        Element *prev = nullptr;
        Element *tmp = head;
        while (tmp->next) {
            prev = tmp;
            tmp = tmp->next;
        }

        tail = prev;
        delete tmp;
        return data;
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