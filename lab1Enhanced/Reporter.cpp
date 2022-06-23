#include "HeaderOfRp.h"

int main(int countArg, char* args[])
{
	std::ifstream f(args[1], std::ios::binary);
	std::vector<employee> vextor;
	while (!f.eof())
	{
		employee e;
		f.read((char*)&e, sizeof(e));
		vextor.push_back(e);
	}
	vextor.pop_back();
	f.close();
	int x = atoi(args[3]);
	std::ofstream out(args[2]);
	out << "File report \"" << args[1]<<"\""<<std::endl;
	for (int i = 0; i < vextor.size(); i++)
	{
		out << vextor[i].num << " " << vextor[i].name << " " << vextor[i].hours << " " << vextor[i].hours * x << std::endl;
	}
	out.close();
}