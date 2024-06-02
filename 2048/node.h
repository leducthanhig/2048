#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <fstream>
using namespace std;

template <typename T>
class node {
    T* data;
    node* next;

public:
    node(T* data);
    node();
    ~node();
    T* getData();
    node* getNext();
    void setData(T* data);
    void setNext(node* next);
    void serialize(fstream& fs, bool bWrite);
};

#endif

template <typename T>
node<T>::node(T* data) {
    this->data = data;
    next = nullptr;
}

template<typename T>
node<T>::node() {
    data = nullptr;
    next = nullptr;
}

template <typename T>
node<T>::~node() {
    delete data;
    next = nullptr;
}

template<typename T>
T* node<T>::getData() {
    return data;
}

template<typename T>
void node<T>::setData(T* data) {
    this->data = data;
}

template<typename T>
void node<T>::setNext(node* next) {
    this->next = next;
}

template<typename T>
node<T>* node<T>::getNext() {
    return next;
}

template<typename T>
void node<T>::serialize(fstream& fs, bool bWrite) {
    data->serialize(fs, bWrite);
}