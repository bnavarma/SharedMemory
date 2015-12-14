#include <cstddef>
#include "pool_allocator.h"

template <class T>
pool_allocator<T>::pool_allocator(void* start, size_t start_off,size_t curr_off, int max) {
	page_start = (size_t)start;
	start_offset = start_off;
	curr_offset = curr_off;
	max_offset = start_offset + (sizeof(T)*max);
}

template <class T>
T* pool_allocator<T>::allocate() {
	size_t temp = curr_offset;
	curr_offset += (sizeof(T));
	if (curr_offset > max_offset) {
		return nullptr;
	}
	return (T*)temp;
}
