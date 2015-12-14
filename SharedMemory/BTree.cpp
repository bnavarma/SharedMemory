#include <cstddef>
#include "pool_allocator.h"
#include "BTree.h"

template<class A, class B>
BTree<A, B>::BTree(size_t root_pointer, void* pg_start, size_t idx_alloc_offset, size_t data_alloc_offset, int max_size) {
	root = (IndexNode<A>*) root_pointer;
	page_start = pg_start;
	idx_allocator = pool_allocator<IndexNode<A>>(pg_start, idx_alloc_offset, idx_alloc_offset + sizeof(void*), max_size);
	temp_leaf_off = idx_alloc_offset + 2 ^ 20;
	leaf_allocator = pool_allocator<LeafNode<A, B>>(pg_start, temp_leaf_off, temp_leaf_off + sizeof(void*), max_size);
	data_allocator = pool_allocator<B>(pg_start, data_alloc_offset, data_alloc_offset + sizeof(void*), max_size);
}

template <class A, class B>
int BTree<A, B>::search(int key, IndexNode<A> node) {
	auto temp = 0;
	for (auto i = 0; i < BranchSize; i++) {
		temp = i;
		if (key >= node->keys[i]) {
			break;
		}
	}
	if (key > node->keys[BranchSize - 1]) {
		temp = BranchSize;
	}
	return temp;
}

template <class A, class B>
int BTree<A, B>::search(int key, LeafNode<A, B>node) {
	for (auto i = 0; i < BranchSize; i++) {
		if (key == node->keys[i]) {
			return i;
		}
		if (key > node->keys[i]) {
			return i;
		}
	}
	throw;
}

template <class A, class B>
void BTree<A, B>::insert(int key, B* value) {
	auto idx = search(key, *root);
	if (root->keys[BranchSize] != NULL) {

	}
}