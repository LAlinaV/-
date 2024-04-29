#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

wchar_t const* mes[3]{ L"R_Exit", L"R_C", L"R_D" };
wchar_t const* EMes[2]{ L"1", L"0" };

int main(int argc, char* argv[])
{
    int k = stol(argv[1]);
    int counter = 0;
    std::string number;
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Semaphore");
    WaitForSingleObject(hSemaphore, INFINITE);
    std::cout << "Process reader is started" << std::endl;
    std::cout << "Process is active" << std::endl;
    HANDLE mesEv[3];
    HANDLE mesEx[2];
    for (int i = 0; i < 3; ++i) { mesEv[i] = OpenEvent(EVENT_ALL_ACCESS, FALSE, mes[i]); }
    for (int i = 0; i < 2; ++i) { mesEx[i] = OpenEvent(EVENT_ALL_ACCESS, FALSE, EMes[i]); }
    while (true)
    {
        DWORD m = WaitForMultipleObjects(3, mesEv, FALSE, INFINITE);
        ResetEvent(mesEv[m]);
        if (m != 0) {
            std::wcout << mes[m] << L'\n';
            counter++;
            if (counter == k)
            {
                SetEvent(mesEx[0]); break;
            }
            else
            {
                SetEvent(mesEx[1]);
            }
        }
    }
    ReleaseSemaphore(hSemaphore, 1, NULL);
    return 0;
}
