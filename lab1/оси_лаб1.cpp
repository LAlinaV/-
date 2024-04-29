#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <process.h>
using namespace std;

UINT WINAPI Worker(void *p)
//DWORD WINAPI Worker(LPVOID pLong) 
{
    cout << "Worker start" << endl;
    long sum = 0;
    pair<int, long*>* temp = (pair<int, long*>*)p;
    
    for (int i = 0; i < (*temp).first; i++)
    {
        sum += ((*temp).second[i] * (*temp).second[i]);
        Sleep(12);
    }
    cout << "Sum " << sum << endl << "Worker end";
    return 0;
}

int main()
{
    int n, timeStart, timeStop;
    cout << "Enter the number of numbers in the array" << endl;
    cin >> n;
    cout << "Enter the numbers in the array" << endl;
    long* arr = new long[n];
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    pair<int, long*> info = make_pair(n, arr);
    cout << "Enter the time after the start of the program to start the thread" << endl;
     cin >> timeStart;
     cout << "Enter the time after the start of the program to stop the thread" << endl;
     cin >> timeStop;
    UINT	IDThread;
    HANDLE hThread;
    //DWORD IDThread;
    hThread = (HANDLE) _beginthreadex(NULL, 0, Worker, &info, CREATE_SUSPENDED , &IDThread);
    //hThread = CreateThread(NULL, 0, Worker, &info, CREATE_SUSPENDED, &IDThread);
    if (hThread == NULL)
        return GetLastError();
    Sleep(timeStart);
            ResumeThread(hThread);
            Sleep(timeStop - timeStart);
            SuspendThread(hThread);
            cout << "The Worker thread is suspended. Enter 'Yes' if you wish to continue" << endl;
            string str;
            while (cin >> str)
            {
                if (str == "Yes")
                {
                    ResumeThread(hThread);
                    break;
                }
                else
                {
                    cout << "The thread is suspended. The program is not completed.Enter 'Yes' if you wish to continue" << endl;
                }
            }
    
    WaitForSingleObject(hThread, INFINITE);
    
    CloseHandle(hThread);
}