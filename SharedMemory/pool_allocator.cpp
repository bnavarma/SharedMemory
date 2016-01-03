#include <cstddef>
#include "pool_allocator.h"

template <class T, int N>
pool_allocator<T, N>::pool_allocator(void* start, size_t start_off) {
	page_start = (size_t)start;
	start_offset = start_off;
	if (*free_list == NULL) {
		*free_list = shared_stack<size_t, N>(start);
	}

	start_offset = start_off + sizeof(size_t);
	cur_offset = (size_t*)start_off;
	if (*cur_offset != NULL) {
		cur_offset = start_offset;
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
		return nullptr;
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