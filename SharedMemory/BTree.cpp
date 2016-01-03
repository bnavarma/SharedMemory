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

template<class A, class B>
void BTree<A, B>::node_split(IndexNode<A>* node) {
	auto temp = idx_allocator.allocate();
	for (auto i = (BranchSize / 2) + 1; i <= BranchSize; i++) {
		if (i != BranchSize) {
			temp->keys[i - (BranchSize / 2) - 1] = node->keys[i];
			node->keys[i] = NULL;
		}
		temp->nodes[i - (BranchSize / 2) - 1] = node->nodes[i];
		node->nodes[i] = NULL;
	}
	node->keys[BranchSize / 2] = NULL;
	temp->next = node->next;
	temp->prev = node;
	node->next = temp;
}

template <class A, class B>
void BTree<A, B>::node_split(LeafNode<A, B>* node) {
	auto temp = leaf_allocator.allocate();
	for (auto i = BranchSize / 2; i < BranchSize; i++) {
		temp->keys[i - (BranchSize / 2)] = node->keys[i];
		temp->nodes[i - (BranchSize / 2)] = node->nodes[i];
		node->keys[i] = NULL;
		node->nodes[i] = NULL;
	}
	temp->next = node->next;
	temp->prev = node;
	node->next = temp;
}

template <class A, class B>
void BTree<A, B>::insert_non_split(int key, IndexNode<A>* node, void* data_node) {
	int idx = search(key, node);
	for (auto i = BranchSize; i > idx; i--) {
		if (i == BranchSize) {
			node->nodes[i] = node->nodes[i - 1];
		}
		else {
			node->keys[i] = node->keys[i - 1];
			node->nodes[i] = node->nodes[i - 1];
		}
	}
	node->keys[idx] = key;
	node->nodes[idx] = data_node;
}

template<class A, class B>
void BTree<A, B>::insert_non_split(int key, LeafNode<A, B>* node, B* data_node) {
	int idx = search(key, node);
	for (auto i = BranchSize - 1; i > idx; i--) {
		node->keys[i] = node->keys[i - 1];
		node->values[i] = node->values[i - 1];
	}
	node->keys[idx] = key;
	node->values[idx] = data_node;
}

template <class A, class B>
void BTree<A, B>::insert_rec(int key, IndexNode<A>* node, void* value) {
	auto idx = search(key, node);
	void* temp = node->nodes[idx];

	IndexNode<A>* idx_node = dynamic_cast<IndexNode<A>*>(temp);
	if (idx_node != NULL) {
		if (idx_node->keys[BranchSize - 1] != NULL) {
			node_split(idx_node);
			insert_non_split(idx_node->next->keys[0], node, idx_node->next);
		}
		if (key > idx_node->next->keys[0]) {
			insert_rec(key, idx_node->next, value);
		}
		else {
			insert_rec(key, idx_node, value);
		}
	}

	LeafNode<A, B>* leaf_node = dynamic_cast<LeafNode<A, B>*>(temp);
	if (leaf_node != NULL) {
		if (leaf_node->keys[BranchSize - 1] != NULL) {
			node_split(leaf_node);
			insert_non_split(leaf_node->next->keys[0], node, leaf_node->next);
		}
		if (key > leaf_node->next->keys[0]) {
			insert_non_split(key, leaf_node->next, value);
		}
		else {
			insert_non_split(key, leaf_node, value);
		}
	}
}

template <class A, class B>
void BTree<A, B>::insert(int key, B* value) {
	if (root->keys[BranchSize] != NULL) {
		node_split(root);
	}
	insert_rec(key, root, value);
}