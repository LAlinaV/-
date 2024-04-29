#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

int main() {
	cout << "Enter the length of the array: ";
	int arrayLength;
	cin >> arrayLength;

	HANDLE hWritePipe, hReadPipe, hEvent, hInheritWritePipe, hInheritReadPipe;
	hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Event");
	if (!hEvent) {
		cout << "-Server-\n Event was not created";
		return 3;
	}

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = FALSE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create pipes
	if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
		cout << "Create pipe failed" << endl;
		return GetLastError();
	}
	if (!DuplicateHandle(

		GetCurrentProcess(), // дескриптор текущего процесса
		hWritePipe, // исходный дескриптор канала
		GetCurrentProcess(), // дескриптор текущего процесса
		&hInheritWritePipe, // новый дескриптор канала
		0, // этот параметр игнорируется
		TRUE, // новый декскриптор наследуемый
		DUPLICATE_SAME_ACCESS))// доступ не изменяем

	{
		_cputs("Duplicate handle failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	// закрываем ненужный дескриптор
	CloseHandle(hWritePipe);

	if (!DuplicateHandle(

		GetCurrentProcess(), // дескриптор текущего процесса
		hReadPipe, // исходный дескриптор канала
		GetCurrentProcess(), // дескриптор текущего процесса
		&hInheritReadPipe, // новый дескриптор канала
		0, // этот параметр игнорируется
		TRUE, // новый декскриптор наследуемый
		DUPLICATE_SAME_ACCESS))// доступ не изменяем

	{
		_cputs("Duplicate handle failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	// закрываем ненужный дескриптор
	CloseHandle(hReadPipe);

	DWORD dwBytesWrite;
	// Write array data to the pipe
	if (!WriteFile(hInheritWritePipe, &arrayLength, sizeof(int), &dwBytesWrite, NULL)) {
		cout << "Write of length failed\n" << GetLastError();
		return GetLastError();
	}


	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t* lpszComLine = new wchar_t[80];
	wsprintf(lpszComLine, L"%p %p", hInheritWritePipe, hInheritReadPipe);
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	
	if (!CreateProcess(L"Hignt.exe", lpszComLine,
		NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE,
		NULL, NULL,
		&si, &pi))
	{
		cout << "Hignt.exe was not launched.\n";
		return GetLastError();
	}


	WaitForSingleObject(hEvent, INFINITE);

	int new_size;
	DWORD byteRwad;
	if (!ReadFile(hInheritReadPipe, &new_size, sizeof(int), &byteRwad, NULL))
	{
		cout << "New size read is failed " << endl;
		return GetLastError();
	}
	__int16* arr1 = new __int16[new_size];
	if (!ReadFile(hInheritReadPipe, arr1, sizeof(__int16) * new_size, &byteRwad, NULL))
	{
		cout << "New array read is failed " << endl;
		return GetLastError();
	}


	for (int i = 0; i < new_size; i++)
	{
		cout << arr1[i] << " ";
	}

	CloseHandle(hInheritReadPipe);
	CloseHandle(hInheritWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


	delete[] arr1;

	return 0;
}