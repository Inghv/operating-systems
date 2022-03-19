#include "SyncQueue.h"

HANDLE start;

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

DWORD WINAPI producer(LPVOID par)
{
	WaitForSingleObject(start, INFINITE);

	DataForProducer* data = (DataForProducer*)par;


	for (int i = 0; i < data->count; i++)
	{
		data->queue->Insert(data->number);
		cout << "Произведено число: " << data->number<< endl;
		Sleep(7);
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
		cout << "Употреблено число: " << number <<endl ;
		Sleep(7);
	}

	return 0;
}

int main()
{
	setlocale(LC_ALL, ".1251");
	int queueSize;
	cout << "Введите размер очереди: ";
	cin >> queueSize;

	start = CreateEvent(NULL, TRUE, FALSE, NULL);

	SyncQueue* queue = new SyncQueue(queueSize);

	int prodThreadCount;
	cout << "Введите количество потоков producer: ";
	cin >> prodThreadCount;

	DataForProducer* workWithProductCountProducer = new DataForProducer[prodThreadCount];
	for (int i = 0; i < prodThreadCount; i++)
	{
		cout << "Введите количество чисел,которые поток должен произвести: ";
		int count;
		cin >> count;
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


	cout << "Введите количество потоков consumer: ";
	int consThreadCount;
	cin >> consThreadCount;

	DataForConsumer* workWithProductCountConsumer = new DataForConsumer[consThreadCount];
	for (int i = 0; i < consThreadCount; i++)
	{
		cout << "Введите количество чисел,которые поток должен произвести: ";
		int count;
		cin >> count;
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

	SetEvent(start);

	WaitForMultipleObjects(prodThreadCount + consThreadCount, handlesArray, TRUE, INFINITE);
	system("pause");
	return 0;
}
