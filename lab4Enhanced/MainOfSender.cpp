#include "HeaderOfSender.h"

void write(char* filename, char* message, char* authorName)
{
	WaitForSingleObject(isEmpty, INFINITE);
	WaitForSingleObject(mutex, INFINITE);

	std::fstream f(filename, std::ios::binary | std::ios::in | std::ios::out);

	int messageWritePos;
	char p[10];
	f.getline(p, sizeof(p));
	messageWritePos = atoi(p);

	Message* m = new Message(authorName, message);
	int pos = sizeof(p) + sizeof(Message) * messageWritePos;
	f.seekp(pos, std::ios::beg);
	f.write((char*)m, sizeof(Message));

	messageWritePos++;
	if (messageWritePos == messagesNumber)
	{
		messageWritePos = 0;
	}
	itoa(messageWritePos, p, 10);
	f.seekp(0, std::ios::beg);
	f.write(p, sizeof(p));

	f.close();

	ReleaseMutex(mutex);
	ReleaseSemaphore(isFull, 1, NULL);
}

int main(int argc, char* argv[])
{
	isFull = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, charArrayToLPCWSTR("Full"));
	isEmpty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, charArrayToLPCWSTR("Empty"));
	mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, charArrayToLPCWSTR("Mutex"));
	char* filename = argv[1];
	messagesNumber = atoi(argv[2]);
	std::cout << "Enter name:" << std::endl;
	char name[20];
	std::cin >> name;

	while (true)
	{
		std::cout << "Enter 1 or 2:" << std::endl;
		std::cout << "1.Write" << std::endl;
		std::cout << "2.Exit" << std::endl;
		int answer;
		std::cin >> answer;

		if (answer == 1)
		{
			std::cout << "Enter message: " << std::endl;
			char message[30];
			std::cin >> message;
			write(filename, message, name);
		}
		else if (answer == 2)
		{
			break;
		}
	}

	delete[] name;
	delete[] filename;
	return 0;
}

