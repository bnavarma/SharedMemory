#pragma once
#include "shared_memory.h"

template <class T, int N>
class shared_stack {
public:
	size_t stack_start;
	size_t stack_end;
	size_t* cur_ptr;
	
	shared_stack(size_t start);

	int push(T input);

	T pop();
};

template <class T, int N>
shared_stack<T, N>::shared_stack(size_t start) {
	stack_start = start;
	cur_ptr = (size_t*)start;
	if (*cur_ptr == NULL) {
		*cur_ptr = start + sizeof(size_t);
	}
	stack_end = start + sizeof(size_t) + sizeof(T)*N;
}

template<class T, int N>
int shared_stack<T, N>::push(T input) {
	auto temp = *cur_ptr + sizeof(T);
	if (temp > stack_end) {
		return -1;
	}
	(T)*cur_ptr = input;
	*cur_ptr = temp;
	return 0;
}

template<class T, int N>
T shared_stack<T, N>::pop() {
	auto temp = *cur_ptr - sizeof(T);
	if (temp < stack_start + sizeof(size_t)) {
		return NULL;
	}
	auto ret = (T)*cur_ptr;
	*cur_ptr = temp;
	return ret;
}