#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;

int main(int argc, char* argv[]) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 4548);
	if (!hProcess) {
		cout << "Failed to open the process\n";
		return GetLastError();
	}
	cout << "[+] Process Opened\n";
	void* alloc = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!alloc) {
		cout << "Failed to allocate a buffer\n";
		return GetLastError();
	}
	cout << "[+] Memory region allocated\n";
	bool wpm = WriteProcessMemory(hProcess, alloc, argv[1], strlen(argv[1]), NULL);
	if (!wpm) {
		cout << "FTW\n";
		return GetLastError();
	}
	HMODULE kernel32 = GetModuleHandleA("Kernel32");
	if (!kernel32) {
		VirtualFreeEx(hProcess, alloc, 0, MEM_RELEASE);
		alloc = NULL;
		return GetLastError();
	}
	FARPROC GetPaddr = GetProcAddress(kernel32, "LoadLibraryA");
	if (!GetPaddr) {
		cout << "GetProcAddress() Fails\n";
		return GetLastError();
	}
	CreateRemoteThread(hProcess, 0, NULL, (LPTHREAD_START_ROUTINE)GetPaddr, alloc, NULL, NULL);
	cout << "i did it\n";

	return 0;
}