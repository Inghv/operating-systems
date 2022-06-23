#include "SyncQueue.h"

SyncQueue::SyncQueue(int size_)
	:posForAdd{ 0 }, posForDelete{ 0 }, size{ size_ }, arr{ new int[size_] }
{
	InitializeCriticalSection(&cs);
	isFull = CreateSemaphore(NULL, 0, size_, NULL);
	isEmpty = CreateSemaphore(NULL, size_, size_, NULL);
}

SyncQueue::~SyncQueue()
{
	DeleteCriticalSection(&cs);
	CloseHandle(isEmpty);
	CloseHandle(isFull);
	delete[]arr;
}

void SyncQueue::Insert(int element)
{
	WaitForSingleObject(isEmpty, INFINITE);

	EnterCriticalSection(&cs);
	arr[posForAdd] = element;
	posForAdd++;
	if (posForAdd == sizeof(arr))
	{
		posForAdd = 0;
	}
	LeaveCriticalSection(&cs);

	ReleaseSemaphore(isFull, 1, NULL);
}

int SyncQueue::Remove()
{
	WaitForSingleObject(isFull, INFINITE);

	EnterCriticalSection(&cs);
	int element = arr[posForDelete];
	posForDelete++;
	if (posForDelete == sizeof(arr))
	{
		posForDelete = 0;
	}
	LeaveCriticalSection(&cs);

	ReleaseSemaphore(isEmpty, 1, NULL);

	return element;
}