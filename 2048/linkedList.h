#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "node.h"
#include <iostream>
using namespace std;

// [Templates in C++ with Examples](https://www.geeksforgeeks.org/templates-cpp)
template <typename T>
struct linkedList {
    node<T> *head;
    int size;

    linkedList();
    ~linkedList();
    void addHead(T val);
	void addNode(int pos, T val);
	void addTail(T val);
	void delHead();
	void delNode(int pos);
	void delTail();
    void delNodeByVal(T val);
    void printList();
    void updateNode(T oldVal, T newVal);
    T getNode(int pos);
};

template <typename T>
linkedList<T>::linkedList() {
    head = NULL;
    size = 0;
}

template <typename T>
linkedList<T>::~linkedList() {
    node<T>* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

template <typename T>
void linkedList<T>::addHead(T val) {
    addNode(0, val);
}

template <typename T>
void linkedList<T>::addNode(int pos, T val) {
    if (pos == 0 || head == NULL) {
        node<T>* newNode = new node<T>(val);
        newNode->next = head;
        head = newNode;
    }
    else {
        node<T>* tmp = head;
        while (tmp->next != NULL && pos > 1) {
            tmp = tmp->next;
            pos--;
        }
        node<T>* newNode = new node<T>(val);
        newNode->next = tmp->next;
        tmp->next = newNode;
    }
    size++;
}

template <typename T>
void linkedList<T>::addTail(T val) {
    addNode(size, val);
}

template <typename T>
void linkedList<T>::delHead() {
    delNode(0);
}

template <typename T>
void linkedList<T>::delNode(int pos) {
    if (head == NULL) return;
    if (pos == 0 || size == 1) {
        node<T>* needDel = head;
        head = head->next;
        delete needDel;
    }
    else {
        node<T>* tmp = head;
        while (tmp->next->next != NULL && pos > 1) {
            tmp = tmp->next;
            pos--;
        }
        node<T>* needDel = tmp->next;
        tmp->next = tmp->next->next;
        delete needDel;
    }
    size--;
}

template <typename T>
void linkedList<T>::delTail() {
    delNode(size - 1);
}

template <typename T>
void linkedList<T>::delNodeByVal(T val) {
    while (head != NULL && head->data == val) {
        node<T>* needDel = head;
        head = head->next;
        delete needDel;
        size--;
    }
    if (head != NULL) {
        node<T>* temp = head;
        while (temp->next->next != NULL) {
            if (temp->next->data == val) {
                node<T>* needDel = temp->next;
                temp->next = temp->next->next;
                delete needDel;
                size--;
            }
            else {
                temp = temp->next;
            }
        }
        if (temp->next->data == val) {
            node<T>* needDel = temp->next;
            temp->next = temp->next->next;
            delete needDel;
            size--;
        }
    }
}

template <typename T>
void linkedList<T>::printList() {
    node<T>* temp = head;
    while (temp != NULL) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

template <typename T>
void linkedList<T>::updateNode(T oldVal, T newVal) {
    node<T>* temp = head;
    while (temp != NULL) {
        if (temp->data == oldVal) {
            temp->data = newVal;
        }
        temp = temp->next;
    }
}

template <typename T>
T linkedList<T>::getNode(int pos) {
    node<T>* temp = head;
    while (temp->next != NULL && pos > 0) {
        pos--;
        temp = temp->next;
    }
    return temp->data;
}

#endif