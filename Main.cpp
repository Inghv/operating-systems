#include <iostream>
#include <Windows.h>
#include <fstream>
#include<string>
using namespace std;
LPWSTR stringToLPWSTR(string com) {
	wchar_t temp[250];
	size_t outSize;
	mbstowcs_s(&outSize, temp, com.c_str(), com.length());
	LPWSTR command = temp;
	return command;
}
int main()
{
	setlocale(LC_ALL, ".1251");
	string filename;
	cout << "¬ведите им€ файла: ";
	cin >> filename;
	string count;
	cout << "¬ведите количество записей: ";
	cin>>count;
	string com = "Creator " + filename + " " + count;

	STARTUPINFO startUpInfo;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&startUpInfo, sizeof(startUpInfo));
	startUpInfo.cb = sizeof(startUpInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));
	if (!CreateProcess(NULL, stringToLPWSTR(com), NULL, NULL, FALSE, 0, NULL, NULL, &startUpInfo, &procInfo)) {
		cout << "CreateProcess is failed";
	}
	WaitForSingleObject(procInfo.hProcess, INFINITE);
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);

	ifstream f(filename);
	string s;
	while (!f.eof()) {
		getline(f, s);
		cout << s << endl;
	}
	f.close();

	string nameOfFileOtcheta;
	string countOfHours;
	cout << "»м€ файла отчЄта: ";
	cin >> nameOfFileOtcheta;
	cout << "ќплата за час работы: ";
	cin >> countOfHours;
	com = "Reporter " + filename + " " + nameOfFileOtcheta + " " + countOfHours;

	ZeroMemory(&startUpInfo, sizeof(startUpInfo));
	startUpInfo.cb = sizeof(startUpInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));
	if (!CreateProcess(NULL, stringToLPWSTR(com), NULL, NULL, FALSE, 0, NULL, NULL, &startUpInfo, &procInfo)) {
		cout << "CreateProcess is failed";
	}
	WaitForSingleObject(procInfo.hProcess, INFINITE);
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
	
	ifstream ff(nameOfFileOtcheta);

	while (!ff.eof()) {
		getline(ff, s);
		cout << s << endl;
	}
	ff.close();

}
