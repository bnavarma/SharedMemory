#pragma once
#include "SharedMemory.h"

template <class T, int N>
class SharedMemory_API shared_stack {
public:
	size_t stack_start;
	size_t stack_end;
	size_t* cur_ptr;

	shared_stack(size_t start);

	int push(T input);

	T pop();
};