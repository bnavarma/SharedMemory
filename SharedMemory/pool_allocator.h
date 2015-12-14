#pragma once

template <class T>
class pool_allocator {
public:
	size_t page_start;
	size_t start_offset;
	size_t max_offset;
	size_t curr_offset;

	pool_allocator(void* start, size_t start_off, size_t curr_off, int max);

	T* allocate();

	void free(void* ptr);
};
