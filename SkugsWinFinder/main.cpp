#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <fstream>
#include <string>

using namespace std;

DWORD GetBaseAddress(const HANDLE hProcess) {
	if (hProcess == NULL)
		return NULL; // No access to the process

	HMODULE lphModule[1024]; // Array that receives the list of module handles
	DWORD lpcbNeeded(NULL); // Output of EnumProcessModules, giving the number of bytes requires to store all modules handles in the lphModule array

	if (!EnumProcessModules(hProcess, lphModule, sizeof(lphModule), &lpcbNeeded))
		return NULL; // Impossible to read modules

	TCHAR szModName[MAX_PATH];
	if (!GetModuleFileNameEx(hProcess, lphModule[0], szModName, sizeof(szModName) / sizeof(TCHAR)))
		return NULL; // Impossible to get module info

	return (DWORD)lphModule[0]; // Module 0 is apparently always the EXE itself, returning its address
}
bool isRunning(HWND hwnd)
{
	if (hwnd != 0) {
		return true;
	}
	else {
		return false;
	}
}
int main()
{
	
	int readP2Win = 0;
	int readP1Win = 0;
	HWND hwnd = FindWindowA(NULL, "Skullgirls Encore");
	if (hwnd == NULL)
	{
		cout << "Cannot Find Skullgirls." << endl;
		Sleep(3000);
		exit(-1);
	}
	else
	{
		DWORD procID;
		GetWindowThreadProcessId(hwnd, &procID);
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

		DWORD AddressOfFirstPointer, ValueAtFirstPointer, AddressOfP1Win, AddressOfP2Win, ValueOfP2Win, ValueOfP1Win;
		DWORD P1WinOffset = 0x434;
		DWORD P2WinOffset = 0x43C;
		DWORD BaseAddress = GetBaseAddress(handle);
		DWORD BaseAddressOffset = 0x00841984;
		
		//Uncomment the print statements for de-bugging reasons!

		//cout << "The Base Address of SkullGirls is: " << std::hex << BaseAddress << endl;

		AddressOfFirstPointer = BaseAddress + BaseAddressOffset;

		//cout << "The Address of The Static Pointer To UI CLass Object is: " << std::hex << AddressOfFirstPointer << endl;

		ReadProcessMemory(handle, (DWORD*)AddressOfFirstPointer, &ValueAtFirstPointer, sizeof(int), 0);

		//cout << "The Value of The Static Pointer To UI CLass Object is: " << std::hex << ValueAtFirstPointer << endl;

		AddressOfP1Win = ValueAtFirstPointer + P1WinOffset;

		//cout << "The Address of The Player 1 Win Bool is: " << std::hex << AddressOfP1Win << endl;

		ReadProcessMemory(handle, (DWORD*)AddressOfP1Win, &ValueOfP1Win, sizeof(int), 0);

		//cout << "The Value of The Player 1 Win Bool is: " << std::hex << ValueOfP1Win << endl;

		AddressOfP2Win = ValueAtFirstPointer + P2WinOffset;

		//cout << "The Address of The Player 2 Win Bool is: " << std::hex << AddressOfP2Win << endl;

		ReadProcessMemory(handle, (DWORD*)AddressOfP2Win, &ValueOfP2Win, sizeof(int), 0);

		//cout << "The Value of The Player 2 Win Bool is: " << std::hex << ValueOfP2Win << endl;

		if (procID == NULL)
		{
			cout << "Cannot Find Skullgirls Proccess ID" << endl;
			Sleep(3000);
			exit(-1);
		}
		else
		{
			cout << "Press Num Pad 0 to Exit..." << endl;
			
			
			int seconds;
			while (true) 
			{
				seconds = 0;
				ReadProcessMemory(handle, (PBYTE*)AddressOfP1Win, &readP1Win, sizeof(int), 0);
				ReadProcessMemory(handle, (PBYTE*)AddressOfP2Win, &readP2Win, sizeof(int), 0);
				
				if (!readP1Win && !readP2Win) 
				{
					string numrounds;
					int numRounds;
					fstream file;
					file.open("C:\\Users\\tanne\\OneDrive\\Documents\\Skullgirls\\Replays_SG2EPlus\\76561198132030993\\round_0001.ini");
					for (int i = 0; i < 4; i++)
					{
						file >> numrounds;
						if (i == 3)
						{
							numRounds = stoi(numrounds);
						}
					}
					file.close();
					while (true) 
					{
						Sleep(1000);
						seconds++;
						cout << "Is running:" << isRunning(FindWindowA(NULL, "Skullgirls Encore")) << endl;
						cout << "Number of rounds" << numRounds << endl;
						ReadProcessMemory(handle, (PBYTE*)AddressOfP1Win, &readP1Win, sizeof(int), 0);
						ReadProcessMemory(handle, (PBYTE*)AddressOfP2Win, &readP2Win, sizeof(int), 0);
						if (isRunning(FindWindowA(NULL, "Skullgirls Encore")))
						{
							if (seconds > 240)
							{
								cout << "No Winner Could Be Found...\n" << endl;
								system("curl --location --request GET localhost:8080 --form winner='None'");
								Sleep(5000);
								break;

							}
							if (readP1Win == numRounds)
							{
								cout << "Player 1 Wins!" << endl;
								system("curl --location --request GET localhost:8080 --form winner='Player1'");
								Sleep(5000);
								break;
							}
							if (readP2Win == numRounds)
							{
								cout << "Player 2 Wins!" << endl;
								system("curl --location --request GET localhost:8080 --form winner='Player2'");
								Sleep(5000);
								break;

							}
							if (!(readP2Win == numRounds) && !(readP1Win == numRounds))
							{
								cout << "Game In Progress..." << endl;

							}
							if (GetAsyncKeyState(VK_NUMPAD0)) { // Exit
								return 0;
							}
						}
						else
						{
							cout << "Replay Caused a Crash!" << endl;
							system("curl --location --request GET localhost:8080 --form winner='RIP'");
							Sleep(10000);
							exit(-1);
						}
					}
				}
			}
			exit(-1);
			}
		}
	}