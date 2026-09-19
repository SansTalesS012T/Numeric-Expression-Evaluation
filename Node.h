#ifndef NODE_H
#define NODE_H

#include <type_traits>

template <typename T>
class Node {
private:
	T data;
	Node *prev;
	Node *next;
public:
	Node(T data) : data(data), prev(nullptr), next(nullptr) {}

	// ~Node() {
	// 	if constexpr (std::is_pointer<T>::value) {
	// 		delete  data;
	// 	}
	// }

	// getter setter
	T get_data() { return data; }
	void set_data(T new_data) { data = new_data; }

	Node<T> *get_prev() { return prev; }
	void set_prev(Node<T> *new_data) { prev = new_data; }

	Node<T> *get_next() { return next; }
	void set_next(Node<T> *new_next) { next = new_next; }
};

#endif