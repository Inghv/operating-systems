#include "SyncQueue.h"

HANDLE start;
DWORD WINAPI producer(LPVOID par);
DWORD WINAPI consumer(LPVOID par);

const int sleepTime = 70;

int main()
{
	int queueSize;
	std::cout << "Enter queue size: ";
	std::cin >> queueSize;

	start = CreateEvent(NULL, TRUE, FALSE, NULL);

	SyncQueue* queue = new SyncQueue(queueSize);

	int prodThreadCount;
	std::cout << "Enter count of thread producer: ";
	std::cin >> prodThreadCount;

	DataForProducer* workWithProductCountProducer = new DataForProducer[prodThreadCount];
	for (int i = 0; i < prodThreadCount; i++)
	{
		std::cout << "Enter the number of numbers the thread should produce: ";
		int count;
		std::cin >> count;
		workWithProductCountProducer[i].count = count;
		workWithProductCountProducer[i].number = i + 1;
		workWithProductCountProducer[i].queue = *&queue;
	}

	HANDLE* prodThreadHandls = new HANDLE[prodThreadCount];
	DWORD* prodThreadID = new DWORD[prodThreadCount];

	for (int i = 0; i < prodThreadCount; i++)
	{
		prodThreadHandls[i] = CreateThread(NULL, 0, producer, (void*)&workWithProductCountProducer[i], 0, &prodThreadID[i]);
	}

	std::cout << "Enter count of thread consumer: ";
	int consThreadCount;
	std::cin >> consThreadCount;

	DataForConsumer* workWithProductCountConsumer = new DataForConsumer[consThreadCount];
	for (int i = 0; i < consThreadCount; i++)
	{
		std::cout << "Enter the number of numbers the thread should consume: ";
		int count;
		std::cin >> count;
		workWithProductCountConsumer[i].count = count;
		workWithProductCountConsumer[i].queue = *&queue;
	}

	HANDLE* consThreadHandls = new HANDLE[consThreadCount];
	DWORD* consThreadID = new DWORD[consThreadCount];

	for (int i = 0; i < consThreadCount; i++)
	{
		consThreadHandls[i] = CreateThread(NULL, 0, consumer, (void*)&workWithProductCountConsumer[i], 0, &consThreadID[i]);
	}

	HANDLE* handlesArray = new HANDLE[prodThreadCount + consThreadCount];

	for (int i = 0; i < prodThreadCount; i++)
	{
		handlesArray[i] = prodThreadHandls[i];
	}

	for (int i = 0; i < consThreadCount; i++)
	{
		handlesArray[i + prodThreadCount] = consThreadHandls[i];
	}

	if (start != 0)
		SetEvent(start);

	WaitForMultipleObjects(prodThreadCount + consThreadCount, handlesArray, TRUE, INFINITE);
	delete[] workWithProductCountProducer;
	delete[] workWithProductCountConsumer;
	delete[] prodThreadID;
	delete[] prodThreadHandls;
	delete[] handlesArray;
	delete[] consThreadHandls;
	delete[] consThreadID;
	queue->~SyncQueue();
	system("pause");
	return 0;
}

DWORD WINAPI producer(LPVOID par)
{
	WaitForSingleObject(start, INFINITE);

	DataForProducer* data = (DataForProducer*)par;

	for (int i = 0; i < data->count; i++)
	{
		data->queue->Insert(data->number);
		std::cout << "Number produced: " << data->number << std::endl;
		Sleep(sleepTime);
	}

	return 0;
}

DWORD WINAPI consumer(LPVOID par)
{
	WaitForSingleObject(start, INFINITE);

	DataForConsumer* data = (DataForConsumer*)par;

	for (int i = 0; i < data->count; i++)
	{
		int number = data->queue->Remove();
		std::cout << "Number consumered: " << number << std::endl;
		Sleep(sleepTime);
	}

	return 0;
}