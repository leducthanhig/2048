#ifndef STACK_H
#define STACK_H

#include "linkedList.h"

template<typename T>
class stack : public linkedList<T> {
public:
    T* top();
    void push(T* val);
    void pop();
    T* pop_r();
    void clear();
};

#endif

template<typename T>
T* stack<T>::top() {
    if (this->head) {
        return this->head->getData();
    }
    return nullptr;
}

template<typename T>
void stack<T>::push(T* val) {
    this->addHead(val);
}

template<typename T>
void stack<T>::pop() {
    this->delHead();
}

template<typename T>
T* stack<T>::pop_r() {
    if (this->head) {
        T* tmp = this->head->getData();
        this->head = this->head->getNext();
        this->size--;
        return tmp;
    }
    return nullptr;
}

template<typename T>
void stack<T>::clear() {
    while (this->head) {
        pop();
    }
}