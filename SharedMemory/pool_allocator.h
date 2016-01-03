#pragma once
#include "SharedMemory.h"
#include "SharedStack.h"

template <class T, int N>
class SharedMemory_API pool_allocator {
public:
	size_t page_start;
	size_t start_offset;
	size_t max_offset;
	size_t* curr_offset;
	shared_stack<size_t, N> *free_list;

	pool_allocator(void* start, size_t start_off);

	T* allocate();

	void free(T* ptr);
};
