#pragma once

template <class T, int N>
class pool_allocator {
public:
	size_t page_start;
	size_t start_offset;
	size_t max_offset;
	size_t curr_offset;
	size_t free_list[N];

	pool_allocator(void* start, size_t start_off, size_t curr_off);

	T* allocate();

	void free(void* ptr);
};
