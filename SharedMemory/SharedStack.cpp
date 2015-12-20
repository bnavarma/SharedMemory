#include <cstddef>
#include "SharedStack.h"

template <class T, int N>
shared_stack<T, N>::shared_stack(size_t start) {
	stack_start = start;
	&cur_ptr = start ;
	if (cur_ptr == NULL) {
		cur_ptr = start + sizeof(size_t);
	}
	stack_end = start +sizeof(size_t)+ sizeof(T)*N;
}

template<class T, int N>
int shared_stack<T, N>::push(T input) {
	auto temp = cur_ptr + sizeof(T);
	if (temp > stack_end) {
		return -1;
	}
	(T)cur_ptr = input;
	cur_ptr = temp;
	return 0;
}

template<class T, int N>
T shared_stack<T, N>::pop() {
	auto temp = cur_ptr - sizeof(T);
	if (temp < start + sizeof(size_t)) {
		return NULL;
	}
	auto ret = (T)cur_ptr;
	cur_ptr = temp;
	return ret;
}