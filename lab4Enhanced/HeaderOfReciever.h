#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

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
		strcpy_s(name, "");
		strcpy_s(text, "");
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

LPWSTR stringToLPWSTR(std::string com)
{
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com.c_str(), com.length());
	LPWSTR command = temp;
	return command;
}
