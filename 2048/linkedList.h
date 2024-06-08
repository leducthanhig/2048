#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

template<typename T>
class linkedList {
protected:
    node<T> *head;
    int size;

public:
    linkedList();
    ~linkedList();
    int getSize();
    node<T>* getHead();
    node<T>* getNode(int pos);
    void addHead(T* val);
	void addNode(int pos, T* val);
	void addTail(T* val);
	void delHead();
	void delNode(int pos);
	void delTail();
    void delNodeByVal(T* val);
    void updateNode(T* oldVal, T* newVal);
    void serialize(fstream& fs, bool bWrite);
    void print();
    bool empty();
    int find(T* nd, bool(*comparator)(T*, T*));
};

#endif

template<typename T>
linkedList<T>::linkedList() {
    head = nullptr;
    size = 0;
}

template<typename T>
linkedList<T>::~linkedList() {
    node<T>* cur;
    while (head) {
        cur = head;
        head = head->getNext();
        delete cur;
        cur = nullptr;
    }
}

template<typename T>
void linkedList<T>::addHead(T* val) {
    addNode(0, val);
}

template<typename T>
void linkedList<T>::addNode(int pos, T* val) {
    if (pos < 0) return;
    if (pos == 0 || head == nullptr) {
        node<T>* newNode = new node<T>(val);
        newNode->setNext(head);
        head = newNode;
    }
    else {
        node<T>* cur = head;
        while (cur->getNext() && pos > 1) {
            cur = cur->getNext();
            pos--;
        }
        node<T>* newNode = new node<T>(val);
        newNode->setNext(cur->getNext());
        cur->setNext(newNode);
    }
    size++;
}

template<typename T>
void linkedList<T>::addTail(T* val) {
    addNode(size, val);
}

template<typename T>
void linkedList<T>::delHead() {
    delNode(0);
}

template<typename T>
void linkedList<T>::delNode(int pos) {
    if (head == nullptr || pos < 0) return;
    if (pos == 0 || size == 1) {
        node<T>* cur = head;
        head = head->getNext();
        delete cur;
        cur = nullptr;
    }
    else {
        node<T>* cur = head;
        while (cur->getNext()->getNext() && pos > 1) {
            cur = cur->getNext();
            pos--;
        }
        node<T>* tmp = cur->getNext();
        cur->setNext(cur->getNext()->getNext());
        delete tmp;
        tmp = nullptr;
    }
    size--;
}

template<typename T>
void linkedList<T>::delTail() {
    delNode(size - 1);
}

template<typename T>
void linkedList<T>::delNodeByVal(T* val) {
    while (head && head->getData() == val) {
        node<T>* cur = head;
        head = head->getNext();
        delete cur;
        cur = nullptr;
        size--;
    }
    if (head) {
        node<T>* cur = head;
        while (cur->getNext()->getNext()) {
            if (cur->getNext()->getData() == val) {
                node<T>* tmp = cur->getNext();
                cur->setNext(cur->getNext()->getNext());
                delete tmp;
                cur = nullptr;
                size--;
            }
            else {
                cur = cur->getNext();
            }
        }
        if (cur->getNext()->getData() == val) {
            node<T>* tmp = cur->getNext();
            cur->setNext(cur->getNext()->getNext());
            delete tmp;
            tmp = nullptr;
            size--;
        }
    }
}

template<typename T>
void linkedList<T>::print() {
    node<T>* cur = head;
    while (cur) {
        cout << cur->getData() << " ";
        cur = cur->getNext();
    }
    cout << endl;
}

template<typename T>
void linkedList<T>::updateNode(T* oldVal, T* newVal) {
    node<T>* cur = head;
    while (cur) {
        if (cur->getData() == oldVal) {
            cur->setData(newVal);
        }
        cur = cur->getNext();
    }
}
//[Reading and writing classes with pointers to binary files in c++](https://stackoverflow.com/a/32833242)
template<typename T>
void linkedList<T>::serialize(fstream& fs, bool bWrite) {
    if (bWrite) {
        fs.write(reinterpret_cast<char*>(&size), sizeof(size));
        node<T>* cur = head;
        while (cur) {
            cur->serialize(fs, bWrite);
            cur = cur->getNext();
        }
    }
    else {
        int size;
        fs.read(reinterpret_cast<char*>(&size), sizeof(size));
        T* data;
        while (size--) {
            data = new T;
            data->serialize(fs, bWrite);
            addTail(data);
        }
    }
}

template<typename T>
bool linkedList<T>::empty() {
    return (size == 0);
}

template<typename T>
int linkedList<T>::getSize() {
    return size;
}

template<typename T>
int linkedList<T>::find(T* nd, bool(*comparator)(T*, T*)) {
    node<T>* cur = head;
    int pos = 0;
    while (cur && !comparator(cur->getData(), nd)) {
        cur = cur->getNext();
        pos++;
    }
    if (cur) return pos;
    return -1;
}

template<typename T>
node<T>* linkedList<T>::getNode(int pos) {
    if (!head || pos < 0) return nullptr;
    node<T>* cur = head;
    while (cur->getNext() && pos > 0) {
        pos--;
        cur = cur->getNext();
    }
    return cur;
}

template<typename T>
node<T>* linkedList<T>::getHead() {
    return head;
}