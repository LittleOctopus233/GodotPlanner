#pragma once

#include <string>
#include <vector>
#include <Windows.h>

struct FileInfo {
	std::wstring filePath;
	std::wstring fileName;
	bool isDir;
};

bool IsDirectoryExists(const std::wstring& dirPath);

void EnumDirectoryFS(const std::wstring& dirPath, std::vector<FileInfo>& fileList, bool recursive = false);

bool CreateFileWithAutoDir(const std::wstring& filePath);

std::wstring SelectFolder(HWND parent, const std::wstring& title);