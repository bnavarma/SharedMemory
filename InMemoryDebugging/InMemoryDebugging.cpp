// InMemoryDebugging.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BTree.h"
#include <Windows.h>

constexpr int BUF_SIZE = 256;
TCHAR szName[] = TEXT("GLOBAL\\TestFileName");

int main()
{
	HANDLE hMapFile;
	void* pBuf;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		BUF_SIZE,
		szName);

	if (hMapFile == NULL) {
		printf("Could not create file mapping object (%d).\n", GetLastError());
		return 1;
	}
	pBuf = MapViewOfFile(hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL) {
		printf("Could not map the view of the file (%d).\n", GetLastError());
		CloseHandle(hMapFile);
		return 1;
	}

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

    return 0;
}

