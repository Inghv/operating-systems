#include "HeaderOfClient.h"

int main(int argc, char* argv[]) {
	HANDLE writePipe = (HANDLE)atoi(argv[1]);
	HANDLE readPipe = (HANDLE)atoi(argv[2]);

	bool doCycle = true;
	while (doCycle) {
		DWORD bytesWrite;
		DWORD bytesRead;
		std::cout << "1) Read" << std::endl;
		std::cout << "2) Change" << std::endl;
		std::cout << "3) Exit" << std::endl;
		int answer;
		std::cin >> answer;
		if (answer == 1)
		{
			int orderNumber;
			std::cout << "Enter count of orders: " << std::endl;
			std::cin >> orderNumber;

			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);
			WriteFile(writePipe, &orderNumber, sizeof(orderNumber), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);
			if (serverAnswer == ORDER_FOUND)
			{
				Order order;
				ReadFile(readPipe, &order, sizeof(order), &bytesRead, NULL);
				std::cout << "Name of order: " << order.name << std::endl;
				std::cout << "Number of product units: " << order.amount << std::endl;
				std::cout << "Unit cost: " << order.price << std::endl;
			}
			else
			{
				std::cout << "Order not found!" << std::endl;
			}

			char c;
			std::cout << "Press any key";
			std::cin >> c;

			WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);
		}

		else if (answer == 2)
		{
			int orderNumber;
			std::cout << "Enter number of order: " << std::endl;
			std::cin >> orderNumber;

			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);

			WriteFile(writePipe, &orderNumber, sizeof(orderNumber), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);

			if (serverAnswer == ORDER_FOUND)
			{
				Order order;

				ReadFile(readPipe, &order, sizeof(order), &bytesRead, NULL);

				WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);

				std::cout << "Name of order: " << order.name << std::endl;
				std::cout << "Number of product units: " << order.amount << std::endl;
				std::cout << "Unit cost: " << order.price << std::endl;

				std::cout << "Enter name of order: " << std::endl;
				std::cin >> order.name;

				std::cout << "Enter number of product units: " << std::endl;
				std::cin >> order.amount;

				std::cout << "Enter unit cost: " << std::endl;
				std::cin >> order.price;

				WriteFile(writePipe, &MODIFY, sizeof(MODIFY), &bytesWrite, NULL);

				WriteFile(writePipe, &order, sizeof(order), &bytesWrite, NULL);
			}
			else
			{
				std::cout << "Order not found!" << std::endl;
			}

			char answer;
			ReadFile(readPipe, &answer, sizeof(answer), &bytesRead, NULL);

			char c;
			std::cout << "Press any key";
			std::cin >> c;

			WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);
		}
		else 
		{
			WriteFile(writePipe, &EXIT, sizeof(EXIT), &bytesWrite, NULL);
			doCycle = false;
		}
	}

	CloseHandle(writePipe);
	CloseHandle(readPipe);
	return 0;
}