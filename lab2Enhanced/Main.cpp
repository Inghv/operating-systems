#include "Polynom.h"

volatile double numeratorsResult;
volatile double denominatorsResult;

DWORD WINAPI Numerator(LPVOID temp);
DWORD WINAPI Denominator(LPVOID temp);

int main() 
{
	int numeratorsDegree;
	std::cout << "Enter numerators degree: " << std::endl;
	std::cin >> numeratorsDegree;

	double* numeratorsCoefficients = new double[numeratorsDegree + 1];
	std::cout << "Enter coefficients of numerator: " << std::endl;
	for (int i = 0; i < numeratorsDegree + 1; i++) 
	{
		std::cin >> numeratorsCoefficients[i];
	}

	int denominatorsDegree;
	std::cout << "Enter denominators degree: " << std::endl;
	std::cin >> denominatorsDegree;

	double* denominatorsCoefficients = new double[denominatorsDegree + 1];
	std::cout << "Enter coefficients of denominator: " << std::endl;
	for (int i = 0; i < denominatorsDegree + 1; i++)
	{
		std::cin >> denominatorsCoefficients[i];
	}

	double x;
	std::cout << "Enter x: ";
	std::cin >> x;

	HANDLE* hThread = new HANDLE[2];
	DWORD* IDThread = new DWORD[2];

	polynom* numerator = new polynom(numeratorsDegree, numeratorsCoefficients, x);
	polynom* denominator = new polynom(denominatorsDegree, denominatorsCoefficients, x);

	hThread[0] = CreateThread(NULL, 0, Numerator, (void*)numerator, 0, &IDThread[0]);
	if (hThread[0] == NULL)
	{
		std::cout << "Error creating thread numerator!";
		return GetLastError();
	}

	hThread[1] = CreateThread(NULL, 0, Denominator, (void*)denominator, 0, &IDThread[1]);
	if (hThread[1] == NULL)
	{
		std::cout << "Error creating thread denominator!";
		return GetLastError();
	}
	
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	
	double result = numeratorsResult/denominatorsResult;
	std::cout << "f(" << x << ") = " << result << std::endl;

	delete[] numeratorsCoefficients;
	delete[] denominatorsCoefficients;
	delete numerator;
	delete denominator;
}

DWORD WINAPI Numerator(LPVOID temp)
{
	polynom numerator = new polynom((polynom*)temp);
	double result = 0.0;
	for (int i = 0; i < numerator.deg + 1; i++)
	{
		result += (numerator.coefficients[i] * pow(numerator.x, numerator.deg - i));
		Sleep(15);
	}
	std::cout << "Numerator p(" << numerator.x << ") = " << result << std::endl;
	numeratorsResult = result;
	return 0;
}

DWORD WINAPI Denominator(LPVOID temp)
{
	polynom denominator = new polynom((polynom*)temp);
	double result = 0.0;
	for (int i = 0; i < denominator.deg + 1; i++)
	{
		result += (denominator.coefficients[i] * pow(denominator.x, denominator.deg - i));
		Sleep(15);
	}
	std::cout << "Denominator p(" << denominator.x << ")= " << result << std::endl;
	denominatorsResult = result;
	return 0;
}