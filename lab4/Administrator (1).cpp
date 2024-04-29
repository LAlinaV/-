#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

using namespace std;

wchar_t const* WriterMes[4]{ L"W_Exit", L"W_A", L"W_B" };
wchar_t const* ReaderMes[4]{ L"R_Exit", L"R_C", L"R_D" };
wchar_t const* ExitMes[2]{ L"1", L"0" };

int main()
{
    HANDLE Writer[3];
    HANDLE Reader[3];
    HANDLE Exit[2];
    for (int i = 1; i < 3; i++)
    {
        Writer[i] = CreateEvent(NULL, FALSE, FALSE, WriterMes[i]);
        Reader[i] = CreateEvent(NULL, TRUE, FALSE, ReaderMes[i]);
    }
    Writer[0] = CreateEvent(NULL, FALSE, FALSE, WriterMes[0]);
    Reader[0] = CreateEvent(NULL, FALSE, FALSE, ReaderMes[0]);

    for (int i = 0; i < 2; ++i) { Exit[i] = CreateEvent(NULL, FALSE, FALSE, ExitMes[i]); }

    int numberOfProcess, numMessages;
    cout << "Input number of process you want to start: ";
    cin >> numberOfProcess;
    cout << "Enter the number of messages to be sent and received: ";
    cin >> numMessages;
    wstring writer = L"Writer.exe", reader = L"Reader.exe";
    wchar_t commandLine[100];
        writer.append(L" ");
        wchar_t buffer[20];
        swprintf(buffer, 20, L"%ld", numMessages);
        writer.append(buffer);
        wcscpy(commandLine, writer.c_str());

        wchar_t commandLine2[100];
        reader.append(L" ");
        wchar_t buffer2[20];
        swprintf(buffer2, 20, L"%ld", numMessages);
        reader.append(buffer2);
        wcscpy(commandLine2, reader.c_str());
    HANDLE hSemaphore = CreateSemaphore(NULL, 2, 2, L"Semaphore");
    HANDLE hMutex = CreateMutex(NULL, 0, L"Mutex");
    STARTUPINFO* Wstp = new STARTUPINFO[numberOfProcess], * Rstp = new STARTUPINFO[numberOfProcess];
    PROCESS_INFORMATION* Wpi = new PROCESS_INFORMATION[numberOfProcess], * Rpi = new PROCESS_INFORMATION[numberOfProcess];

    for (int i = 0; i < numberOfProcess; i++)
    {
        ZeroMemory(&Wstp[i], sizeof(STARTUPINFO));
        Wstp[i].cb = sizeof(STARTUPINFO);
        ZeroMemory(&Rstp[i], sizeof(STARTUPINFO));
        Rstp[i].cb = sizeof(STARTUPINFO);
        CreateProcess(NULL, commandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Wstp[i], &Wpi[i]);
        CreateProcess(NULL, commandLine2, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Rstp[i], &Rpi[i]);
    }

    int counter = 1;
    for (int i = 0; i < numberOfProcess; i++)
    {
        while (true)
        {
            DWORD mes = WaitForMultipleObjects(3, Writer, FALSE, INFINITE);
            SetEvent(Reader[mes]);
            wcout << WriterMes[mes] << endl;
            DWORD mes1 = WaitForMultipleObjects(2, Exit, FALSE, INFINITE);
            if (mes1 == WAIT_OBJECT_0)
            {
                cout << "Reader " << counter << " ended his work\n";
                counter += 1;
                break;
            }
        }
        cout << "Writer " << i + 1 << " ended his work\n";
    }

    for (int i = 0; i < 3; i++)
    {
        CloseHandle(Writer[i]);
        CloseHandle(Reader[i]);
    }

    for (int i = 0; i < 2; i++) { CloseHandle(Exit[i]); }

    for (int i = 0; i < numberOfProcess; i++)
    {
        CloseHandle(Wpi[i].hThread);
        CloseHandle(Wpi[i].hProcess);
        CloseHandle(Rpi[i].hThread);
        CloseHandle(Rpi[i].hProcess);
    }

    CloseHandle(hSemaphore);
    return 0;
}
