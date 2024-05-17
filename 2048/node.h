#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
using namespace std;

// [Templates in C++ with Examples](https://www.geeksforgeeks.org/templates-cpp)
template <typename T>
struct node {
    T data;
    node* next;

    node(T);
    ~node();
};

template <typename T>
node<T>::node(T val) {
    data = val;
    next = NULL;
}

template <typename T>
node<T>::~node() {
    /*
        Copilot:
            [is_pointer Template in C++](https://www.geeksforgeeks.org/is_pointer-template-in-cpp)
            [std::is_pointer](https://en.cppreference.com/w/cpp/types/is_pointer)
            [Template parameters and template arguments](https://en.cppreference.com/w/cpp/language/template_parameters)
    */
    if constexpr (is_pointer<T>::value) {
        delete data;
    }
    next = NULL;
}

#endif