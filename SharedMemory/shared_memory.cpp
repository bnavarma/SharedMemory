// SharedMemory.cpp : Defines the exported functions for the DLL application.
//

#include "shared_memory.h"

// This is an example of an exported variable
SharedMemory_API int nSharedMemory = 0;

// This is an example of an exported function.
SharedMemory_API int fnSharedMemory(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see shared_memory.h for the class definition
c_shared_memory::c_shared_memory()
{
	return;
}
