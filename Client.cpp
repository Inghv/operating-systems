#include <windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

char READ = 'r';
char MODIFY = 'm';
char ORDER_NOT_FOUND = 'n';
char ORDER_FOUND = 'f';
char EXIT = 'e';
char END_OPERATION = 'd';
char END_MODIFY = 'n';

struct Order {
	int n;
	char name[10];
	int amount;
	double price;
};

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, ".1251");
	HANDLE writePipe = (HANDLE)atoi(argv[1]);
	HANDLE readPipe = (HANDLE)atoi(argv[2]);
	bool doCycle = true;
	while (doCycle) {
		DWORD bytesWrite;
		DWORD bytesRead;
		cout << "1) Прочесть" << endl;
		cout << "2)Изменить"<< endl;
		cout<<"3)Выход"<<endl;
		int answer;
		cin >> answer;
		system("cls");
		if (answer == 1) {		
			int orderNumber;
			cout << "Введите число заказов: "<<endl;
			cin >> orderNumber;

			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);
			WriteFile(writePipe, &orderNumber, sizeof(orderNumber), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);
			if (serverAnswer == ORDER_FOUND) {
				Order order;
				ReadFile(readPipe, &order, sizeof(order), &bytesRead, NULL);
				cout << "Название заказа: " << order.name << endl;
				cout << "Количество единиц товара: " << order.amount << endl;
				cout << "Стоимость единицы товара: " << order.price << endl;
			} else {
				cout << "Заказ не найден!"<<endl;
			}

			char c;
			cout << "Нажмите любую клавишу";
			cin >> c;

			WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);
		} else if (answer == 2) {
			int orderNumber;
			cout << "Введите номер заказа: "<<endl;
			cin >> orderNumber;

			WriteFile(writePipe, &READ, sizeof(READ), &bytesWrite, NULL);

			WriteFile(writePipe, &orderNumber, sizeof(orderNumber), &bytesWrite, NULL);

			char serverAnswer;
			ReadFile(readPipe, &serverAnswer, sizeof(serverAnswer), &bytesRead, NULL);

			if (serverAnswer == ORDER_FOUND) {
				Order order;

				ReadFile(readPipe, &order, sizeof(order), &bytesRead, NULL);

				WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);

				cout << "Название заказа: " << order.name << endl;
				cout << "Количество товара: " << order.amount << endl;
				cout << "Стоимость единицы товара: " << order.price << endl;

				cout << "Введите название заказа: "<<endl;
				cin >> order.name;
				
				cout << "Введите количество единиц товара: "<<endl;
				cin >> order.amount;
				
				cout << "Введите стоимость единицы товара: "<<endl;
				cin >> order.price;

				WriteFile(writePipe, &MODIFY, sizeof(MODIFY), &bytesWrite, NULL);

				WriteFile(writePipe, &order, sizeof(order), &bytesWrite, NULL);		
			} else {
				cout << "Заказ не найден!"<<endl;
			}

			char answer;
			ReadFile(readPipe, &answer, sizeof(answer), &bytesRead, NULL);

			char c;
			cout << "Нажмите любую клавишу";
			cin >> c;

			WriteFile(writePipe, &END_OPERATION, sizeof(END_OPERATION), &bytesWrite, NULL);
		} else {
			WriteFile(writePipe, &EXIT, sizeof(EXIT), &bytesWrite, NULL);
			doCycle = false;
	    }
	}

	CloseHandle(writePipe);
	CloseHandle(readPipe);
	return 0;
}

