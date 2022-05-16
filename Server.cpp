#include <iostream>
#include <fstream>
#include <Windows.h>

#pragma warning(disable : 4996)

using namespace std;

char READ = 'r';
char MODIFY = 'm';
char ORDER_NOT_FOUND = 'n';
char ORDER_FOUND = 'f';
char EXIT = 'e';
char END_OPERATION = 'd';
char END_MODIFY = 'n';
char filename[20];

volatile int readerCount = 0;
CRITICAL_SECTION cs;
HANDLE semaphore;

struct Order {
	int n;
	char name[10];
	int amount;
	double price;
};

Order* findOrder(int orderNumber) {
	ifstream in(filename, ios::binary);
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

void modify(Order order) {
	fstream f(filename, ios::binary | ios::in | ios::out);
	int pos = 0;
	int orderSize = sizeof(Order);
	while (f.peek() != EOF)
	{
		Order o;
		f.read((char*)&o, sizeof(Order));
		if (order.n == o.n) {
			f.seekp(pos * orderSize, ios::beg);
			f.write((char*)&order, sizeof(Order));
			f.close();
			return;
		}
		else {
			pos++;
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
		cout << "��������� ����� �� ������!"<<endl;
		system("pause");
		return 0;
	}

	if (!CreatePipe(&clientReadPipe, &writePipe, &sa, 0)) {
		cout << "��������� ����� �� ������!"<<endl;
		system("pause");
		return 0;
	}

	char comLine[333];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


	sprintf(comLine, "Client %d %d", (int)clientWritePipe, (int)clientReadPipe);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	cout << comLine;

	if (!CreateProcess(NULL, comLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "����� ������� �� ��� ������!"<<endl;
		return 0;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	bool doCycle = true;
	while (doCycle) {
		DWORD bytesRead;
		DWORD bytesWrite;

		char clientWant;
		ReadFile(readPipe, &clientWant, sizeof(clientWant), &bytesRead, NULL);

		if (clientWant == READ) {
			EnterCriticalSection(&cs);
			++readerCount;
			if (readerCount == 1) {
				WaitForSingleObject(semaphore, INFINITE);
			}
			LeaveCriticalSection(&cs);

			int orderNumber;
			ReadFile(readPipe, &orderNumber, sizeof(int), &bytesRead, NULL);
			Order* order = findOrder(orderNumber);

			if (order != nullptr) {
				WriteFile(writePipe, &ORDER_FOUND, sizeof(ORDER_FOUND), &bytesWrite, NULL);
				WriteFile(writePipe, order, sizeof(Order), &bytesWrite, NULL);
			}
			else {
				WriteFile(writePipe, &ORDER_NOT_FOUND, sizeof(ORDER_NOT_FOUND), &bytesWrite, NULL);
			}

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);

			EnterCriticalSection(&cs);
			--readerCount;
			if (readerCount == 0) {
				ReleaseSemaphore(semaphore, 1, NULL);
			}
			LeaveCriticalSection(&cs);
		}
		else if (clientWant == MODIFY) {
			WaitForSingleObject(semaphore, INFINITE);

			Order order;
			ReadFile(readPipe, &order, sizeof(Order), &bytesRead, NULL);
			modify(order);
			WriteFile(writePipe, &END_MODIFY, sizeof(END_MODIFY), &bytesWrite, NULL);

			char end;
			ReadFile(readPipe, &end, sizeof(end), &bytesRead, NULL);

			ReleaseSemaphore(semaphore, 1, NULL);
		}
		else {
			doCycle = false;
		}
	}
	return 0;
}

void createBinaryFile() {
	ofstream out(filename, ios::binary);

	cout << "������� ������� ������� �� ������ ��������: "<<endl;
	int num;
	cin >> num;

	for (int i = 0; i < num; i++)
	{
		system("cls");
		Order order;
		cout << "������� ����� ������: "<<endl;
		cin >> order.n;

		cout << "������� ��� ������: "<<endl;
		cin >> order.name;

		cout << "������� ���������� ������: "<<endl;
		cin >> order.amount;

		cout << "������� ��������� ������� ������: "<<endl;
		cin >> order.price;

		out.write((char*)&order, sizeof(struct Order));
	}

	out.close();
}

void checkDataInBinaryFile()
{
	ifstream in(filename, ios::binary);
	system("cls");
	cout << "������: "<<endl;
	while (in.peek() != EOF)
	{
		Order order;
		in.read((char*)&order, sizeof(Order));
		cout << "����� ������: " << order.n << endl;
		cout << "�������� ������: " << order.name << endl;
		cout << "���������� ������: " << order.amount << endl;
		cout << "��������� ������� ������: " << order.price << endl;
	}

	in.close();
}

void main() {
	setlocale(LC_ALL, ".1251");
	InitializeCriticalSection(&cs);
	semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	cout << "������� �������� ����� � ��������: "<<endl;
	cin >> filename;

	createBinaryFile();
	checkDataInBinaryFile();

	cout << "������� ���������� ������� client: "<<endl;
	int clientCount;
	cin >> clientCount;
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

	system("pause");
}