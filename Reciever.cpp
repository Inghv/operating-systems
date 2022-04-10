#include <Windows.h>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

using namespace std;

HANDLE isFull;
HANDLE isEmpty;
HANDLE mutex;
int messageReadPos = 0;
int messagesNumber;

struct Message {
	char name[10];
	char text[20];

	Message() 
	{
		strcpy(name, "");
		strcpy(text, "");
	}

	Message(char* name1, char* text1)
	{
		strcpy_s(name, name1);
		strcpy_s(text, text1);
	}

	Message(const char* name1, const char* text1)
	{
		strcpy_s(name, name1);
		strcpy_s(text, text1);
	}
};

LPCWSTR charArrayToLPCWSTR(const char* com)
{
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com, sizeof(com));
	LPCWSTR command = temp;
	return command;
}

LPWSTR stringToLPWSTR(string com)
{
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com.c_str(), com.length());
	LPWSTR command = temp;
	return command;
}


void createFile(string filename, int messagesNumber)
{
	ofstream fout(filename, ios::binary);
	int pos = 0;
	char p[10];
	itoa(pos, p, 10);
	fout.write(p, sizeof(p));
	Message* m = new Message("empty", "empty");
	for (int i = 0; i < messagesNumber; i++) {
		fout.write((char*)&m, sizeof(Message));
	}
	fout.close();
}

void read(string filename)
{
	WaitForSingleObject(isFull, INFINITE);
	WaitForSingleObject(mutex, INFINITE);
	//cout << "Позиция,откуда читать файл: " << messageReadPos << endl;

	fstream f(filename, ios::binary | ios::in | ios::out);
	if (!f.is_open())
	{
		cout << "Ошибка открытия файла!"<<endl;
		return;
	}

	Message* m = new Message();
	char writeIter[10];
	int pos = sizeof(writeIter) + sizeof(Message) * messageReadPos;

	f.seekg(pos, ios::beg);
	f.getline((char*)m, sizeof(Message));
	cout << "Автор сообщения: " << m->name << ", текст: " << m->text << endl;

	f.seekp(pos, ios::beg);
	m = new Message("удалено", "удалено");
	f.write((char*)m, sizeof(Message));

	messageReadPos++;
	if (messageReadPos == messagesNumber) 
	{
		messageReadPos = 0;
	}

	f.close();

	ReleaseMutex(mutex);
	ReleaseSemaphore(isEmpty, 1, NULL);
}

void main() 
{
	setlocale(LC_ALL, ".1251");
	cout << "Введите имя файла: "<<endl;
	string filename;
	cin >> filename;

	cout << "Введите количество сообщений:"<<endl;
	cin >> messagesNumber;
	createFile(filename, messagesNumber);

	cout << "Введите количество процессов Sender: "<<endl;
	int senderProcessCount;
	cin >> senderProcessCount;

	isFull = CreateSemaphore(NULL, 0, messagesNumber, charArrayToLPCWSTR("Full"));
	isEmpty = CreateSemaphore(NULL, messagesNumber, messagesNumber, charArrayToLPCWSTR("Empty"));
	mutex = CreateMutex(NULL, FALSE, charArrayToLPCWSTR("Mutex"));

	STARTUPINFO* si = new STARTUPINFO[senderProcessCount];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[senderProcessCount];



	char num[10];
	itoa(messagesNumber, num, 10);
	string data = "Sender " + filename + " " + num;

	for (int i = 0; i < senderProcessCount; i++)
	{
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		cout << data << endl;
		if (!CreateProcess(NULL, stringToLPWSTR(data), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]))
		{
			cout << "Новый процесс не был создан."<<endl;
			return;
		}
	}

	system("cls");
	
	while (true)
	{
		cout << "Введите 1 или 2:" << endl;
		cout << "1.Читать" << endl;
		cout << "2.Выход" << endl; 
		int answer;
		cin >> answer;

		string str;
		if (answer == 1) {
			
			read(filename);
		}
		else if (answer == 2) {
			break;
		}
	}

}