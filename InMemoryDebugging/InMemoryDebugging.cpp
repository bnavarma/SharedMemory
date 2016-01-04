// InMemoryDebugging.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "b_tree.h"
#include <Windows.h>
#include <conio.h>

constexpr int BUF_SIZE = 256;
TCHAR szName[] = TEXT("GLOBAL\\TestFileName");

int main()
{
	HANDLE hMapFile;
	void* pBuf;
	int ret_val = 0;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		BUF_SIZE,
		szName);

	if (hMapFile == NULL) {
		printf("Could not create file mapping object (%d).\n", GetLastError());
		ret_val = 1;
	}
	else {
		pBuf = MapViewOfFile(hMapFile,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			BUF_SIZE);

		if (pBuf == NULL) {
			printf("Could not map the view of the file (%d).\n", GetLastError());
			CloseHandle(hMapFile);
			ret_val = 1;
		}
		else {
			UnmapViewOfFile(pBuf);
			CloseHandle(hMapFile);
		}
	}

	printf("Press any key to exit...");
	_getch();

	return ret_val;
}
