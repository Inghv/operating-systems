#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>

struct employee
{
	int num;
	char name[10];
	double hours;
};

LPWSTR stringToLPWSTR(std::string com)
{
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com.c_str(), com.length());
	LPWSTR command = temp;
	return command;
}
