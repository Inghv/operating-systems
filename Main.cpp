#include <iostream>
#include <fstream>
#include <windows.h>
#include <iomanip>

using namespace std;

volatile double numeratorsResult;
volatile double denominatorsResult;

struct polynom
{
	int deg;
	double* coefficients;
	double x;

	polynom(int deg_, double* coefficients_, double x_)
		:deg{ deg_ }, coefficients{ coefficients_ },x{x_}
	{}

	polynom(polynom* t)
		:deg{t->deg},coefficients{t->coefficients},x{t->x}
	{}
};

DWORD WINAPI Numerator(LPVOID temp)
{
	polynom numerator = new polynom((polynom*)temp);
	double result = 0.0;
	for (int i = 0; i < numerator.deg + 1; i++) {
		result += (numerator.coefficients[i] * pow(numerator.x, numerator.deg - i));
		Sleep(15);
	}
	cout << "„ислитель p("<<numerator.x<<") = " << result << endl;
	numeratorsResult = result;
	return 0;
}


DWORD WINAPI Denominator(LPVOID temp)
{
	polynom denominator = new polynom((polynom*)temp);
	double result = 0.0;
	for (int i = 0; i < denominator.deg + 1; i++) {
		result += (denominator.coefficients[i] * pow(denominator.x, denominator.deg - i));
		Sleep(15);
	}
	cout << "«наменатель p(" << denominator.x << ")= " << result << endl;
	denominatorsResult = result;
	return 0;
}

int main() 
{
	setlocale(LC_ALL, ".1251");
	int numeratorsDegree;
	cout << "¬ведите степень полинома-числител€: " << endl;
	cin >> numeratorsDegree;

	double* numeratorsCoefficients = new double[numeratorsDegree + 1];
	cout << "¬ведите коэффициенты полинома-числител€: "<<endl;
	for (int i = 0; i < numeratorsDegree + 1; i++) {
		cin >> numeratorsCoefficients[i];
	}

	int denominatorsDegree;
	cout << "¬ведите степень полинома-знаменател€: "<<endl;
	cin >> denominatorsDegree;

	double* denominatorsCoefficients = new double[denominatorsDegree + 1];
	cout << "¬ведите коэффициенты полинома-знаменател€: "<<endl;
	for (int i = 0; i < denominatorsDegree + 1; i++) {
		cin >> denominatorsCoefficients[i];
	}

	double x;
	cout << "¬ведите значение переменной x: ";
	cin >> x;

	HANDLE* hThread = new HANDLE[2];
	DWORD* IDThread = new DWORD[2];

	polynom* numerator = new polynom(numeratorsDegree, numeratorsCoefficients, x);
	polynom* denominator = new polynom(denominatorsDegree, denominatorsCoefficients, x);

	hThread[0] = CreateThread(NULL, 0, Numerator, (void*)numerator, 0, &IDThread[0]);
	if (hThread[0] == NULL) {
		cout << "error";
		return GetLastError();
	}

	hThread[1] = CreateThread(NULL, 0, Denominator, (void*)denominator, 0, &IDThread[1]);
	if (hThread[1] == NULL) {
		cout << "error";
		return GetLastError();
	}
	
	
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	
	double result = numeratorsResult/denominatorsResult;
	cout << "f("<<x<<") = " << result << endl;
	
}