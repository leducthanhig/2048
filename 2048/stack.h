#ifndef _STACK_H_
#define _STACK_H_

#include "linkedList.h"
#include <iostream>
using namespace std;

// [Templates in C++ with Examples](https://www.geeksforgeeks.org/templates-cpp)
template <typename T>
struct stack {
    linkedList<T>* l;

    stack();
    ~stack();
    T top();
    void push(T val);
    void pop();
    T pop_r();
    void clear();
    void print();
    bool empty();
    int size();
};

template <typename T>
stack<T>::stack() {
    l = new linkedList<T>;
}

template <typename T>
stack<T>::~stack() {
    delete l;
}

template <typename T>
T stack<T>::top() {
    if (l->head) {
        return l->head->data;
    }
    return T();
}

template <typename T>
void stack<T>::push(T val) {
    l->addHead(val);
}

template <typename T>
void stack<T>::pop() {
    l->delHead();
}

template <typename T>
T stack<T>::pop_r() {
    if (l->head) {
        T tmp = l->head->data;
        l->head = l->head->next;
        l->size--;
        return tmp;
    }
    return T();
}

template <typename T>
void stack<T>::clear() {
    delete l;
    l = new linkedList<T>;
}

template <typename T>
void stack<T>::print() {
    l->printList();
}

template <typename T>
bool stack<T>::empty() {
    return l->head == NULL;
}

template<typename T>
inline int stack<T>::size() {
    return l->size;
}

#endif