#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;
const int MAX_SIZE = 100;

CRITICAL_SECTION cs;
HANDLE event1;

DWORD WINAPI WorkThread(LPVOID param) {
	cout << "Start work\n";
	vector<string>* array = static_cast<vector<string>*>(param);
	int t;
	cout << "Enter t: ";
	cin >> t;

	vector<string> result;

	for (const auto& lexeme : *array) {
		if (std::isdigit(lexeme[0])) {
			result.insert(result.begin(), lexeme);
		}
		else {
			result.push_back("_");
		}
		Sleep(t);
	}

	for (int i = 0; i < result.size(); i++) {
		(*array)[i] = result[i];
	}
	cout << "End work\n";
	SetEvent(event1);

	return 0;
}

DWORD WINAPI CountElementThread(LPVOID param) {
	EnterCriticalSection(&cs);
	cout << "Start section\n";
	std::vector<string>* array = static_cast<std::vector<string>*>(param);
	WaitForSingleObject(event1, INFINITE);

	int count = 0;
	
	for (int i = 0; i < array->size(); ++i) {
		if ((*array)[i] != "_") {
			++count;
		}
	}
	cout << "End section\n";
	LeaveCriticalSection(&cs);

	return count;
}


int main() {
	InitializeCriticalSection(&cs);
	event1 = CreateEvent(NULL, TRUE, FALSE, NULL);

	std::cout << "Enter array size: \n";
	int arraySize;
	std::cin >> arraySize;
	string input = "";
	cout << "Enter elements\n";
	std::getline(cin, input);
	std::getline(cin, input);
	std::vector<std::string> lexemes;

	std::string currentLexeme;
	for (char c : input) {
		if (c != ' ' && c != '-') {
			currentLexeme += c;
		}
		else{
			if (!currentLexeme.empty()) {
				lexemes.push_back(currentLexeme);
				currentLexeme.clear();
			}
		}
	}
	if (!currentLexeme.empty()) {
		lexemes.push_back(currentLexeme);
	}

	for (const auto& lexeme : lexemes) {
		std::cout << lexeme << " ";
	}
	cout << std::endl;

	HANDLE workThread = CreateThread(NULL, 0, WorkThread, &lexemes, 0, NULL);
	HANDLE countThread = CreateThread(NULL, 0, CountElementThread, &lexemes, 0, NULL);

	

	WaitForSingleObject(event1, INFINITE);

	std::cout << "Result after work thread: ";
	for (int i = 0; i < arraySize; ++i) {
		std::cout << lexemes[i] << " ";
	}
	std::cout << std::endl;

	DWORD count;
	EnterCriticalSection(&cs); // Entering the critical section for safe output of the result
	GetExitCodeThread(countThread, &count);
	
	std::cout << "Number of elements start with number: " << count << std::endl;
	LeaveCriticalSection(&cs); // Leaving the critical section

	CloseHandle(event1);
	CloseHandle(countThread);
	DeleteCriticalSection(&cs);

	return 0;
}
