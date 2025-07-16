#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <stdio.h>

template <typename T>
class Node {
private:
    T data;
    Node *prev, *next;
public:
    Node(T newData) {
        data = newData;
        prev = next = nullptr;
    }

    // getter setter
    T getData() { return data; }
    void setData(T newData) { data = newData; }

    Node<T> *getPrev() { return prev; }
    void setPrev(Node<T> *newPrev) { prev = newPrev; }

    Node<T> *getNext() { return next; }
    void setNext(Node<T> *newNext) { next = newNext; }
};
#endif