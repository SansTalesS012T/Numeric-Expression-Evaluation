#include "Node.h"


template <typename T>
class LinkedList {
private:
    Node<T> *head, *tail;
    int size;
public:
    LinkedList() { 
        head = tail = nullptr; 
        size = 0; 
    }

    Node<T> *getHead() { return head; }

    int getSize() { return size; }

    bool isEmpty() { return size == 0; }

    bool isValidRange(int index) { return 0 <= index && index < size; }

    void print() {
        Node<T> *temp = head;
        while(temp) {
            std::cout << temp->getData() << ",";
            temp = temp->getNext();
        }
        std::cout << std::endl;
    }

    void appendNode(Node<T> *newNode) {
        if(isEmpty()) {
            head = tail = newNode;
        }
        else if(size == 1) {
            tail = newNode;
            head->setNext(tail);
            tail->setPrev(head);
        }
        else {
            newNode->setPrev(tail);
            tail->setNext(newNode);
            tail = newNode;
        }
        size++;
    }

    void append(T newData) {
        Node<T> *newNode = new Node<T>(newData);
        appendNode(newNode);
    }

    void pop() {
        if(isEmpty()) return ;
        tail = tail->getPrev();
        delete tail->getNext();
        size--;
    }

    Node<T> *getNodeAt(int index) {
        if(!isValidRange(index)) return nullptr;
        Node<T> *i;
        int nowIndex, sign;
        if(index <= size/2) i = head, nowIndex = 0, sign = 1;
        else i = tail, nowIndex = size - 1, sign = -1;
        while(nowIndex != index) {
            if(sign == 1) i = i->getNext();
            else i = i->getPrev();
            nowIndex += sign;
        }
        return i;
    }

    void setItem(int index, double newData) {
        Node<T> *temp = getNodeAt(index);
        if(temp == nullptr) return ;
        temp->setData(newData);
    }

    void insert(int index, double newData) {
        if(!isValidRange(index)) return ;
        Node<T> *newNode = new Node<T>(), *temp, *before;
        newNode->setData(newData);
        temp = getNodeAt(index);
        before = temp->getPrev();
        if(before) {
            before->setNext(newNode);
            newNode->setPrev(before);
        }
        newNode->setNext(temp);
        temp->setPrev(newNode);
        if(index == 0) head = newNode;
        size++;
    }

    void remove(int index) {
        if(index == size - 1) {
            pop();
            return ;
        }
        Node<T> *temp = getNodeAt(index), *before, *after;
        if(temp == nullptr) return ;
        before = temp->getPrev();
        after = temp->getNext();
        if(before) {
            before->setNext(after);
            after->setPrev(before);
        }
        delete temp;
        if(index == 0) head = after;
        size--;
    }
};

class Pair {
private:
    std::string expression;
    double result;
public:
    Pair(std::string x, double y) {
        expression = x;
        result = y;
    }

    std::string getVal1() { return expression; }
    void setVal1(std::string newVal1) { expression = newVal1; }

    double getVal2() { return result; }
    void setVal2(double newVal2) { result = newVal2; }

    std::string getData() { return expression + " = " + std::to_string(result); }
};