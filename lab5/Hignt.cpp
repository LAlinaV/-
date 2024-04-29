#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>


using namespace std;

int main(int argc, char* argv[]) {
    
    HANDLE hReadPipe, hWritePipe;
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Event");
    if (!hEvent) {
        cout << "Event was not read";
    }
    sscanf(argv[0], "%p", &hWritePipe);
    sscanf(argv[1], "%p", &hReadPipe);
    
    int size;

    DWORD dwBytesWrite, dwBytesRead;

    
    if (!ReadFile(hReadPipe, &size, sizeof(int), &dwBytesRead, NULL)) {
        cout << "array  read is failed " << endl;
        return GetLastError();
    }

    __int16 N;
    cout << "Enter N ";
    cin >> N;

    // Dynamic allocation of array
    __int16* arr = new __int16[size];
    srand(time(0)); // инициализация генератора случайных чисел
    int colvo = 0;
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
        // генерация случайного числа от 0 до 99
    }

    cout << "Generated Array:" << endl;
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

   vector<__int16> res(0);
    for (int i = 0; i < size; i++) {
        if (arr[i] > N) {
            colvo++;
            res.push_back(arr[i]);
        }
    }
    int new_size = colvo;
    __int16* arr1 = new __int16[new_size];

    
        for (int i = 0; i < new_size; ++i) {
            cout << res[i] << " ";
            arr1[i] = res[i];
        }
    
    cout << endl;
    SetEvent(hEvent);
    

    if (!WriteFile(hWritePipe, &new_size, sizeof(int), &dwBytesWrite, NULL)) {
        cout << "Write of new size failed\n";
        return GetLastError();
    }
    if (!WriteFile(hWritePipe, arr1, new_size * sizeof(__int16), &dwBytesWrite, NULL)) {
        cout << "Write of new array failed\n";
        return GetLastError();
    }

    _cprintf("\nTo exit press any key ");
    _getch();

    delete[] arr;
    delete[] arr1;
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);

    return 0;
}