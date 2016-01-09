#pragma once
#include "shared_memory.h"
#include "pool_allocator.h"

template <class A, class B>
class b_tree {
public:
	IndexNode<A>  *root;
	void* page_start;
	pool_allocator<IndexNode<A>, MaxSize>* idx_allocator;
	pool_allocator<LeafNode<A, B>, MaxSize>* leaf_allocator;
	pool_allocator<B, MaxSize>* data_allocator;

	b_tree(void* pg_start, size_t tree_start, size_t alloc_start);

	int search(int key, IndexNode<A>* node);

	int search(int key, LeafNode<A, B>* node);

	void node_split(IndexNode<A>* node);

	void node_split(LeafNode<A, B>* node);

	void insert_non_split(int key, IndexNode<A>* node, void* data_node);

	void insert_non_split(int key, LeafNode<A, B>* node, B* value);

	void insert_rec(int key, IndexNode<A>* node, void* value);

	void insert(int key, B* value);
};

template<class A, class B>
b_tree<A, B>::b_tree(void* pg_start, size_t tree_start, size_t alloc_start) {
	page_start = pg_start;
	root = (IndexNode<A>*) tree_start;
	idx_allocator = &pool_allocator<IndexNode<A>, MaxSize>(pg_start, alloc_start);

	size_t temp_leaf_off = alloc_start + 2 ^ 10 + MaxSize*(sizeof(IndexNode<A>) + sizeof(size_t));
	leaf_allocator = &pool_allocator<LeafNode<A,B>, MaxSize>(pg_start, temp_leaf_off);

	size_t data_leaf_off = temp_leaf_off + 2 ^ 10 + MaxSize*(sizeof(LeafNode<A, B>) + sizeof(size_t));
	data_allocator = &pool_allocator<B, MaxSize>(pg_start, data_leaf_off);

	if (*((int*)root) == NULL) {
		root = idx_allocator->allocate();
		insert(0, NULL);
	}
}

template <class A, class B>
int b_tree<A, B>::search(int key, IndexNode<A>* node) {
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
int b_tree<A, B>::search(int key, LeafNode<A, B>* node) {
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
void b_tree<A, B>::node_split(IndexNode<A>* node) {
	auto temp = idx_allocator->allocate();
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
void b_tree<A, B>::node_split(LeafNode<A, B>* node) {
	auto temp = leaf_allocator->allocate();
	for (auto i = BranchSize / 2; i < BranchSize; i++) {
		temp->keys[i - (BranchSize / 2)] = node->keys[i];
		temp->values[i - (BranchSize / 2)] = node->values[i];
		node->keys[i] = NULL;
		node->values[i] = NULL;
	}
	temp->next = node->next;
	temp->prev = node;
	node->next = temp;
}

template <class A, class B>
void b_tree<A, B>::insert_non_split(int key, IndexNode<A>* node, void* data_node) {
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
	node->nodes[idx] = static_cast<Node*>(data_node);
}

template<class A, class B>
void b_tree<A, B>::insert_non_split(int key, LeafNode<A, B>* node, B* data_node) {
	int idx = search(key, node);
	for (auto i = BranchSize - 1; i > idx; i--) {
		node->keys[i] = node->keys[i - 1];
		node->values[i] = node->values[i - 1];
	}
	node->keys[idx] = key;
	node->values[idx] = data_node;
}

template <class A, class B>
void b_tree<A, B>::insert_rec(int key, IndexNode<A>* node, void* value) {
	auto idx = search(key, node);
	Node* temp = node->nodes[idx];

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
			insert_non_split(key, leaf_node->next, static_cast<B*>(value));
		}
		else {
			insert_non_split(key, leaf_node, static_cast<B*>(value));
		}
	}
}

template <class A, class B>
void b_tree<A, B>::insert(int key, B* value) {
	if (root->keys[BranchSize] != NULL) {
		node_split(root);
	}
	insert_rec(key, root, value);
}