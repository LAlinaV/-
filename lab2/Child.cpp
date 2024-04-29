#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char* argv[])
{
	_cputs("Child is created.\n");
	long sum = 0;
	for (int i = 1; i < (long)argc; i++)
	{
		sum += (std::stol(argv[i]) * std::stol(argv[i]));
		_cprintf("\n My %d parameter = %s", i, argv[i]);
		Sleep(1000);
	}
	_cprintf("\n Sum = %d", sum);
	//Sleep(10000);
	_cputs("\nPlease, press button to exit program\n");
	_getch();

    return 0;
}