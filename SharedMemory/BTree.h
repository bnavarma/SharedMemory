#pragma once

constexpr int BranchSize = 50;
constexpr int MaxSize = 100;

template <class T>
struct IndexNode {
	IndexNode* prev;
	IndexNode* next;
	T keys[BranchSize];
	void* nodes[BranchSize + 1];
};

template <class A, class B>
struct LeafNode {
	LeafNode* prev;
	LeafNode* next;
	A keys[BranchSize];
	B* values[BranchSize];
};

template <class A, class B>
class BTree {
public:
	IndexNode<A>  *root;
	void* page_start;
	pool_allocator<IndexNode<A>,MaxSize> idx_allocator;
	pool_allocator<LeafNode<A, B>, MaxSize> leaf_allocator;
	pool_allocator<B, MaxSize> data_allocator;

	BTree(size_t root_pointer, void* pg_start, size_t idx_alloc_offset, size_t data_alloc_offset, int max_size);

	int search(int key, IndexNode<A> node);

	int search(int key, LeafNode<A, B> node);

	void node_split(IndexNode<A>* node);

	void node_split(LeafNode<A, B>* node);

	void insert_non_split(int key, IndexNode<A>* node, void* data_node);

	void insert_non_split(int key, LeafNode<A, B>* node, B* value);

	void insert_rec(int key, IndexNode<A>* node, void* value);

	void insert(int key, B* value);
};