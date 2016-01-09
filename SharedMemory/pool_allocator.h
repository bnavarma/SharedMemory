#pragma once
#include "shared_memory.h"
#include "shared_stack.h"

template <class T, int N>
class pool_allocator {
public:
	size_t page_start;
	size_t start_offset;
	size_t max_offset;
	size_t *curr_offset;
	shared_stack<size_t, N> *free_list;
	
	pool_allocator(void* start, size_t start_off);

	T* allocate();

	void free(T* ptr);
};

template <class T, int N>
pool_allocator<T, N>::pool_allocator(void* start, size_t start_off) {
	page_start = (size_t)start;
	if (free_list == NULL) {
		shared_stack<size_t, N> temp_stack(start_off);
		*free_list  = temp_stack;
	}

	start_offset = start_off + sizeof(size_t)*(N + 1);
	curr_offset = (size_t*)start_off;
	if (*curr_offset != NULL) {
		*curr_offset = start_offset;
	}
	max_offset = start_offset + (sizeof(T)*N);
}

template <class T, int N>
T* pool_allocator<T, N>::allocate() {
	size_t temp = free_list->pop();
	if (temp != NULL) {
		return (T*)temp;
	}

	temp = *curr_offset;
	*curr_offset += (sizeof(T));
	if (*curr_offset > max_offset) {
		return NULL;
	}
	return (T*)temp;
}

template <class T, int N>
void pool_allocator<T, N>::free(T* ptr) {
	int ret = free_list->push((size_t)ptr);
	if (ret != 0) {
		throw;
	}
}