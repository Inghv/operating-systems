#include <iostream>
#include<fstream>
using namespace std;

struct employee
{
	int num;
	char name[10];
	double hours;
};

int main(int countArg, char* args[])
{
	setlocale(LC_ALL, ".1251");
	ofstream f(args[1], ios::binary);
	int n = atoi(args[2]);
	for (int i = 0; i < n; i++)
	{
		employee empl;
		cout << "������� ID: ";
		cin >> empl.num;
		cout << "������� ���: ";
		cin >> empl.name;
		cout << "������� ���������� �����: ";
		cin >> empl.hours;
		f.write((char*)&empl, sizeof(empl));
	}
	f.close();
}