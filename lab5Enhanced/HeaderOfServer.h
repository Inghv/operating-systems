#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>

#pragma warning(disable : 4996)

char READ = 'r';
char MODIFY = 'm';
char ORDER_NOT_FOUND = 'n';
char ORDER_FOUND = 'f';
char EXIT = 'e';
char END_OPERATION = 'd';
char END_MODIFY = 'n';
char filename[20];

volatile int readerCount = 0;
CRITICAL_SECTION cs;
HANDLE semaphore;

struct Order
{
	int n;
	char name[10];
	int amount;
	double price;
};

LPWSTR charArrayToLPWSTR(const char* com)
{
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com, sizeof(com));
	LPWSTR command = temp;
	return command;
}