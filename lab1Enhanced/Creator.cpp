#include "HeaderOfCr.h"

int main(int countArg, char* args[])
{
	std::ofstream f(args[1], std::ios::binary);
	int n = atoi(args[2]);
	for (int i = 0; i < n; i++)
	{
		employee empl;
		std::cout << "Enter ID: ";
		std::cin >> empl.num;
		std::cout << "Enter name: ";
		std::cin >> empl.name;
		std::cout << "Enter count of hours: ";
		std::cin >> empl.hours;
		f.write((char*)&empl, sizeof(empl));
	}
	f.close();
}