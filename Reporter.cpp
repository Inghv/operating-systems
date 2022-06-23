#include <iostream>
#include <fstream>
#include <vector>

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
	ifstream f(args[1], ios::binary);
	vector<employee> vextor;
	while (!f.eof()) {
		employee e;
		f.read((char*)&e, sizeof(e));
		vextor.push_back(e);
	}
	vextor.pop_back();
	f.close();
	int x = atoi(args[3]);
	ofstream out(args[2]);
	out << "Отчёт по файлу \"" << args[1]<<"\""<<endl;
	for (int i = 0; i < vextor.size(); i++)
	{
		out << vextor[i].num << " " << vextor[i].name << " " << vextor[i].hours << " " << vextor[i].hours * x << endl;
	}
	out.close();
}