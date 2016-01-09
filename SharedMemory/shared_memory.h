#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SharedMemory_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SharedMemory_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SharedMemory_EXPORTS
#define SharedMemory_API __declspec(dllexport)
#else
#define SharedMemory_API __declspec(dllimport)
#endif

// This is an example of a class exported from the SharedMemory.dll
class SharedMemory_API c_shared_memory
{
public:
	c_shared_memory();
	// TODO: add your methods here.
};

// This is an example of an exported variable
extern SharedMemory_API int nSharedMemory;

// This is an example of an exported function.
SharedMemory_API int fnSharedMemory(void);

constexpr int BranchSize = 5;
constexpr int MaxSize = 100;

struct Node{
	virtual ~Node(){}
};

template <class T>
struct IndexNode : Node{
	IndexNode<T>* prev;
	IndexNode<T>* next;
	T keys[BranchSize];
	Node* nodes[BranchSize + 1];
};

template <class A, class B>
struct LeafNode : Node{
	LeafNode<A,B>* prev;
	LeafNode<A,B>* next;
	A keys[BranchSize];
	B* values[BranchSize];
};