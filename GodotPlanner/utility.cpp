#include "utility.h"

#include <windows.h>
#include <conio.h>

std::wstring Utf8ToWide(const std::string& utf8) {
	if (utf8.empty()) return L"";
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wstr[0], len);
	wstr.resize(len - 1);
	return wstr;
}

std::string WideToUtf8(const std::wstring& wstr) {
	if (wstr.empty()) return "";
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8(len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8[0], len, nullptr, nullptr);
	utf8.resize(len - 1);
	return utf8;
}

void WriteLineW(const std::wstring& wstr) {
	DWORD written;
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wstr.c_str(), (DWORD)wstr.length(), &written, NULL);
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, &written, NULL);
}

void WriteW(const std::wstring& wstr) {
	DWORD written;
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wstr.c_str(), (DWORD)wstr.length(), &written, NULL);
}

void ShowConsole() {
	AllocConsole();
	FILE* pFile;
	freopen_s(&pFile, "CONOUT$", "w", stdout);
	freopen_s(&pFile, "CONIN$", "r", stdin);
}
