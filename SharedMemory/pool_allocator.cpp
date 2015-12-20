#include <cstddef>
#include "pool_allocator.h"

template <class T, int N>
pool_allocator<T, N>::pool_allocator(void* start, size_t start_off, size_t curr_off) {
	page_start = (size_t)start;
	start_offset = start_off;
	curr_offset = curr_off;
	max_offset = start_offset + (sizeof(T)*N);
}

template <class T, int N>
T* pool_allocator<T, N>::allocate() {
	size_t temp = curr_offset;
	curr_offset += (sizeof(T));
	if (curr_offset > max_offset) {
		return nullptr;
	}
	return (T*)temp;
}
