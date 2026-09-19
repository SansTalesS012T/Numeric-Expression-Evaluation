#ifndef STACK_GUARD
#define STACK_GUARD

using namespace std;

#include "Node.h"
#include <iostream>

template <typename T>
class Stack {
private:
	Node<T> *top;
	unsigned int size;
public:
	Stack() : top(nullptr), size(0) {}

	~Stack() {
		while(!is_empty()) 
			pop();
	}

	void clear() { 
		while(!is_empty()) 
			pop(); 
	}

	bool is_empty() { return size == 0; }

	void push(T new_data) {
		Node<T> *node = new Node<T>(new_data);
		push_node(node);
	}

	T pop() {
		if(is_empty())
			throw out_of_range("Stack is empty: underflow.");
		
		Node<T> *del_node 	= top;
		T data 				= del_node->get_data();

		size--;
		top = top->get_next();
		delete del_node;
		return data;
	}

	T peek() {
		if(is_empty())
			throw out_of_range("Stack is empty: no element.");

		return top->get_data();
	}

private:
	void push_node(Node<T> *node) {
		size++;
		node->set_next(top);
		top = node;
	}
};

#endif