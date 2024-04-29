#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

wchar_t const* mes[3]{ L"W_Exit", L"W_A", L"W_B" };

int main(int argc, char* argv[])
{
    int k = stol(argv[1]);
    int counter = 0;
    std::string line;
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"Mutex");
    WaitForSingleObject(hMutex, INFINITE);

    std::cout << "Process writer is started" << std::endl;
    std::cout << "Process is active" << std::endl;

    HANDLE mesEv[3];
    for (int i = 0; i < 3; ++i) { mesEv[i] = OpenEvent(EVENT_MODIFY_STATE, FALSE, mes[i]); }
    while (true)
    {
        std::cin >> line;
        line = "W_" + line;
        wstring wline(line.begin(), line.end());
        if (wline._Equal(mes[1])) { SetEvent(mesEv[1]); }
        if (wline._Equal(mes[2])) { SetEvent(mesEv[2]); }
        counter++;
        if (counter == k)
        {
            SetEvent(mesEv[0]); break;
        }
    }
    ReleaseMutex(hMutex);
    return 0;
}
