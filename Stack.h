#include "Node.h"

template <typename T>
class Stack {
private:
    Node<T> *top;
    int size;
public:
    Stack() {
        top = nullptr;
        size = 0;
    }

    void clear() { while(!isEmpty()) pop(); }

    bool isEmpty() { return size == 0; }

    void pushNode(Node<T> *newNode) {
        if(isEmpty()) top = newNode;
        else {
            newNode->setNext(top);
            top = newNode;
        }
        size++;
    }

    void push(T newData) {
        Node<T> *newNode = new Node<T>(newData);
        pushNode(newNode);
    }

    void pop() {
        if(isEmpty()) return ;
        Node<T> *before = top;
        top  = top->getNext();
        delete before;
        size--;
    }

    T peek() {
        if(isEmpty()) return 0;
        return top->getData();
    }
};
