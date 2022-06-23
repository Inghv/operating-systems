#include <iostream>
#include <fstream>
#include <windows.h>
#include <iomanip>

const int sleepTime = 15;

struct polynom
{
	int deg;
	double* coefficients;
	double x;
	polynom(int deg_, double* coefficients_, double x_);
	polynom(polynom* t);
};
