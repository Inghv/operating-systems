#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

HANDLE isFull;
HANDLE isEmpty;
HANDLE mutex;
int messagesNumber;

LPWSTR charArrayToLPWSTR(const char* com)
{
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com, sizeof(com));
	LPWSTR command = temp;
	return command;
}

LPCWSTR charArrayToLPCWSTR(const char* com)
{
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com, sizeof(com));
	LPCWSTR command = temp;
	return command;
}

struct Message
{
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
};