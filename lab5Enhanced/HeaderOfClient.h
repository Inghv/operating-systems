#pragma once
#include <windows.h>
#include <conio.h>
#include <iostream>

#pragma warning(disable : 4996)

char READ = 'r';
char MODIFY = 'm';
char ORDER_NOT_FOUND = 'n';
char ORDER_FOUND = 'f';
char EXIT = 'e';
char END_OPERATION = 'd';
char END_MODIFY = 'n';

struct Order
{
	int n;
	char name[10];
	int amount;
	double price;
};