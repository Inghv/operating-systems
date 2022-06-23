#pragma once
#include <iostream>
#include <Windows.h>
#include <time.h>

class SyncQueue
{
private:
	int* arr;
	int size;
	int posForAdd;
	int posForDelete;
	HANDLE isFull;
	HANDLE isEmpty;
	CRITICAL_SECTION cs;
public:
	SyncQueue(int size_);
	~SyncQueue();
	void Insert(int element);
	int Remove();
};

struct DataForProducer
{
	SyncQueue* queue;
	int number;
	int count;
};

struct DataForConsumer
{
	SyncQueue* queue;
	int count;
};