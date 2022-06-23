#include "HeaderOfReciever.h"

void createFile(std::string filename, int messagesNumber)
{
	std::ofstream fout(filename, std::ios::binary);
	int pos = 0;
	char p[10];
	itoa(pos, p, 10);
	fout.write(p, sizeof(p));
	Message* m = new Message("Empty", "Empty");
	for (int i = 0; i < messagesNumber; i++)
	{
		fout.write((char*)&m, sizeof(Message));
	}
	fout.close();
}

void read(std::string filename)
{
	WaitForSingleObject(isFull, INFINITE);
	WaitForSingleObject(mutex, INFINITE);

	std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);
	if (!f.is_open())
	{
		std::cout << "Open file error!" << std::endl;
		return;
	}

	Message* m = new Message();
	char writeIter[10];
	int pos = sizeof(writeIter) + sizeof(Message) * messageReadPos;

	f.seekg(pos, std::ios::beg);
	f.getline((char*)m, sizeof(Message));
	std::cout << "Author: " << m->name << ", text: " << m->text << std::endl;

	f.seekp(pos, std::ios::beg);
	m = new Message("Deleted", "Deleted");
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

int main()
{
	std::cout << "Enter file name: " << std::endl;
	std::string filename;
	std::cin >> filename;

	std::cout << "Enter count of messages:" << std::endl;
	std::cin >> messagesNumber;
	createFile(filename, messagesNumber);

	std::cout << "Enter count of thread Sender: " << std::endl;
	int senderProcessCount;
	std::cin >> senderProcessCount;

	isFull = CreateSemaphore(NULL, 0, messagesNumber, charArrayToLPCWSTR("Full"));
	isEmpty = CreateSemaphore(NULL, messagesNumber, messagesNumber, charArrayToLPCWSTR("Empty"));
	mutex = CreateMutex(NULL, FALSE, charArrayToLPCWSTR("Mutex"));

	STARTUPINFO* si = new STARTUPINFO[senderProcessCount];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[senderProcessCount];

	char num[10];
	itoa(messagesNumber, num, 10);
	std::string data = "Sender " + filename + " " + num;

	for (int i = 0; i < senderProcessCount; i++)
	{
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		std::cout << data << std::endl;
		if (!CreateProcess(NULL, stringToLPWSTR(data), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]))
		{
			std::cout << "New process was not created!" << std::endl;
			return 0;
		}
	}

	while (true)
	{
		std::cout << "Enter 1 or 2:" << std::endl;
		std::cout << "1.Read" << std::endl;
		std::cout << "2.Exit" << std::endl;
		int answer;
		std::cin >> answer;

		std::string str;
		if (answer == 1)
		{

			read(filename);
		}
		else if (answer == 2)
		{
			break;
		}
	}

	delete[] num;
	delete[] si;
	delete[] pi;
	return 0;
}



