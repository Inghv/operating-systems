#include "HeaderOfMn.h"

int main()
{
	std::string filename;
	std::cout << "Enter filename: ";
	std::cin >> filename;
	std::string count;
	std::cout << "Enter number of records: ";
	std::cin>>count;
	std::string com = "Creator " + filename + " " + count;

	STARTUPINFO startUpInfo;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&startUpInfo, sizeof(startUpInfo));
	startUpInfo.cb = sizeof(startUpInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));
	if (!CreateProcess(NULL, stringToLPWSTR(com), NULL, NULL, FALSE, 0, NULL, NULL, &startUpInfo, &procInfo))
	{
		std::cout << "CreateProcess is failed!";
	}
	WaitForSingleObject(procInfo.hProcess, INFINITE);
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);

	std::ifstream f(filename);
	std::string s;
	while (!f.eof())
	{
		getline(f, s);
		std::cout << s << std::endl;
	}
	f.close();

	std::string reportFileName;
	std::string countOfHours;
	std::cout << "Report file name: ";
	std::cin >> reportFileName;
	std::cout << "Pay per hour of work: ";
	std::cin >> countOfHours;
	com = "Reporter " + filename + " " + reportFileName + " " + countOfHours;

	ZeroMemory(&startUpInfo, sizeof(startUpInfo));
	startUpInfo.cb = sizeof(startUpInfo);
	ZeroMemory(&procInfo, sizeof(procInfo));
	if (!CreateProcess(NULL, stringToLPWSTR(com), NULL, NULL, FALSE, 0, NULL, NULL, &startUpInfo, &procInfo))
	{
		std::cout << "CreateProcess is failed!";
	}
	WaitForSingleObject(procInfo.hProcess, INFINITE);
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
	
	std::ifstream ff(reportFileName);

	while (!ff.eof())
	{
		getline(ff, s);
		std::cout << s << std::endl;
	}
	ff.close();
}
