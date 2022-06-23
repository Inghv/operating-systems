#include "HeaderOfServer.h"

Order* findOrder(int orderNumber)
{
	std::ifstream in(filename, std::ios::binary);
	while (in.peek() != EOF)
	{
		Order* order = new Order;
		in.read((char*)order, sizeof(Order));
		if (order->n == orderNumber) {
			in.close();
			return order;
		}
	}

	in.close();
	return nullptr;
}

void modify(Order order)
{
	std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);
	int pos = 0;
	int orderSize = sizeof(Order);
	while (f.peek() != EOF)
	{
		Order o;
		f.read((char*)&o, sizeof(Order));
		if (order.n == o.n) {
			f.seekp(pos * orderSize, std::ios::beg);
			f.write((char*)&order, sizeof(Order));
			f.close();
			return;
		}
		else
		{
			++pos;
		}
	}
}

DWORD WINAPI client(LPVOID data)
{
	HANDLE writePipe;
	HANDLE readPipe;
	HANDLE clientReadPipe;
	HANDLE clientWritePipe;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&readPipe, &clientWritePipe, &sa, 0)) {
		std::cout << "Pipe was not created!" << std::endl;
		system("pause");
		return 0;
	}

	if (!CreatePipe(&clientReadPipe, &writePipe, &sa, 0)) {
		std::cout << "Pipe was not created!" << std::endl;
		system("pause");
		return 0;
	}

	char comLine[333];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


	sprintf(comLine, "Client %d %d", (int)clientWritePipe, (int)clientReadPipe);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	std::cout << comLine;

	if (!CreateProcess(NULL, charArrayToLPWSTR(comLine), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		std::cout << "New process was not created!" << std::endl;
		return 0;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	bool doCycle = true;
	while (doCycle)
	{
		DWORD bytesRead;
		DWORD bytesWrite;

		char clientWant;
		ReadFile(readPipe, &clientWant, sizeof(clientWant), &bytesRead, NULL);

		if (clientWant == READ)
		{
			EnterCriticalSection(&cs);
			++readerCount;
			if (readerCount == 1)
			{
				WaitForSingleObject(semaphore, INFINITE);
			}
			LeaveCriticalSection(&cs);

			int orderNumber;
			ReadFile(readPipe, &orderNumber, sizeof(int), &bytesRead, NULL);
			Order* order = findOrder(orderNumber);

			if (order != nullptr)
			{
				WriteFile(writePipe, &ORDER_FOUND, sizeof(ORDER_FOUND), &bytesWrite, NULL);
				WriteFile(writePipe, order, sizeof(Order), &bytesWrite, NULL);
			}
			else
			{
				WriteFile(writePipe, &ORDER_NOT_FOUND, sizeof(ORDER_NOT_FOUND), &bytesWrite, NULL);
			}

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);

			EnterCriticalSection(&cs);
			--readerCount;
			if (readerCount == 0)
			{
				ReleaseSemaphore(semaphore, 1, NULL);
			}
			LeaveCriticalSection(&cs);
		}
		else if (clientWant == MODIFY)
		{
			WaitForSingleObject(semaphore, INFINITE);

			Order order;
			ReadFile(readPipe, &order, sizeof(Order), &bytesRead, NULL);
			modify(order);
			WriteFile(writePipe, &END_MODIFY, sizeof(END_MODIFY), &bytesWrite, NULL);

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);

			ReleaseSemaphore(semaphore, 1, NULL);
		}
		else
		{
			doCycle = false;
		}
	}
	return 0;
}

void createBinaryFile()
{
	std::ofstream out(filename, std::ios::binary);

	std::cout << "Enter count of orders: " << std::endl;
	int num;
	std::cin >> num;

	for (int i = 0; i < num; i++)
	{
		system("cls");
		Order order;
		std::cout << "Enter number of order: " << std::endl;
		std::cin >> order.n;

		std::cout << "Enter order name: " << std::endl;
		std::cin >> order.name;

		std::cout << "Enter number of product units: " << std::endl;
		std::cin >> order.amount;

		std::cout << "Enter unit cost: " << std::endl;
		std::cin >> order.price;

		out.write((char*)&order, sizeof(struct Order));
	}

	out.close();
}

void checkDataInBinaryFile()
{
	std::ifstream in(filename, std::ios::binary);
	system("cls");
	std::cout << "Orders: " << std::endl;
	while (in.peek() != EOF)
	{
		Order order;
		in.read((char*)&order, sizeof(Order));
		std::cout << "Number of order: " << order.n << std::endl;
		std::cout << "Name of order: " << order.name << std::endl;
		std::cout << "Number of product units: " << order.amount << std::endl;
		std::cout << "Unit cost: " << order.price << std::endl;
	}

	in.close();
}

int main()
{
	InitializeCriticalSection(&cs);
	semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	std::cout << "Enter file with orders name: " << std::endl;
	std::cin >> filename;

	createBinaryFile();
	checkDataInBinaryFile();

	std::cout << "Enter count of thread client: " << std::endl;
	int clientCount;
	std::cin >> clientCount;
	system("cls");
	HANDLE* handles = new HANDLE[clientCount];
	DWORD* ID = new DWORD[clientCount];
	for (int i = 0; i < clientCount; i++)
	{
		handles[i] = CreateThread(NULL, NULL, client, (void*)i, 0, &ID[i]);
	}
	WaitForMultipleObjects(clientCount, handles, TRUE, INFINITE);
	system("cls");
	checkDataInBinaryFile();

	delete[] handles;
	delete[] ID;
	system("pause");
	return 0;
}