#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
using namespace std;
int main()
{
	int size;
	int x;
    std::cout << "Please,input array size:\n";
    std::cin >> size;


    std::wstring title;
    std::wstring temp = L"Child.exe";

    wchar_t commandLine[100];
    wchar_t childTitle[100];


    long* array = new long[size];

    std::cout << "Please,input array elements:\n";
    for (int i = 0; i < size; ++i)
    {
        std::cin >> array[i];
    }

    std::cout << "Please,input Child title:\n";
    std::wcin >> title;

    for (int i = 0; i < size; i++)
    {
        temp.append(L" ");

        wchar_t buffer[20];
        swprintf(buffer, 20, L"%ld", array[i]);
        temp.append(buffer);
    }
	std::cout << "Please,input X:\n";
	std::cin >> x;
    wcscpy(commandLine, temp.c_str());
    wcscpy(childTitle, title.c_str());


	STARTUPINFO si;
	PROCESS_INFORMATION piApp;


	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwX = x;
	si.dwFlags = STARTF_USEPOSITION;

	
	// создаем новый консольный процесс
	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}

	_cputs("The new process is created.\n");
	// ждем завершения созданного прцесса
	WaitForSingleObject(piApp.hProcess, INFINITE);
	// закрываем дескрипторы этого процесса в текущем процессе
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	return 0;
}



