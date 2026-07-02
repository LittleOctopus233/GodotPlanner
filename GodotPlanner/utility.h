#pragma once

#include <string>

std::wstring Utf8ToWide(const std::string& utf8);

std::string WideToUtf8(const std::wstring& wstr);

void WriteLineW(const std::wstring& wstr);

void WriteW(const std::wstring& wstr);

void ShowConsole();

void ShowBasicInfo();