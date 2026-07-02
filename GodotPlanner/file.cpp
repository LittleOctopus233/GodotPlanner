#include "file.h"

#include <filesystem>
#include <fstream>
#include <windows.h>
#include <shobjidl.h>

namespace fs = std::filesystem;

bool IsDirectoryExists(const std::wstring& dirPath) {
	std::error_code ec;
	return fs::is_directory(dirPath, ec);
}

void EnumDirectoryFS(const std::wstring& dirPath, std::vector<FileInfo>& fileList, bool recursive) {
	if(!fs::exists(dirPath) || !fs::is_directory(dirPath)) return;
	if(recursive) {
		for(const auto& entry : fs::recursive_directory_iterator(dirPath))
			fileList.push_back({entry.path().wstring(), entry.path().filename().wstring(), entry.is_directory()});
	}
	else {
		for(const auto& entry : fs::directory_iterator(dirPath))
			fileList.push_back({entry.path().wstring(), entry.path().filename().wstring(), entry.is_directory()});
	}
}

bool CreateFileWithAutoDir(const std::wstring& filePath) {
	try {
		fs::path file_path(filePath);
		fs::path parent_dir = file_path.parent_path();

		if(!parent_dir.empty()) {
			fs::create_directories(parent_dir);
		}

		std::ofstream file(file_path, std::ios::out | std::ios::trunc);
		if(!file.is_open()) {
			return false;
		}

		file.close();
		return true;

	}
	catch(const fs::filesystem_error&) {
		return false;
	}
	catch(const std::ofstream::failure&) {
		return false;
	}
}

std::wstring SelectFolder(HWND parent, const std::wstring& title) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	bool needUninit = SUCCEEDED(hr);

	IFileDialog* pfd = nullptr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileDialog, reinterpret_cast<void**>(&pfd));
	if(FAILED(hr)) {
		if(needUninit) CoUninitialize();
		return L"";
	}

	DWORD options;
	pfd->GetOptions(&options);
	pfd->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);

	pfd->SetTitle(title.c_str());

	hr = pfd->Show(parent);
	if(FAILED(hr)) {
		pfd->Release();
		if(needUninit) CoUninitialize();
		return L"";
	}

	IShellItem* psi = nullptr;
	hr = pfd->GetResult(&psi);
	if(FAILED(hr)) {
		pfd->Release();
		if(needUninit) CoUninitialize();
		return L"";
	}

	wchar_t* path = nullptr;
	hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &path);
	if(FAILED(hr) || !path) {
		psi->Release();
		pfd->Release();
		if(needUninit) CoUninitialize();
		return L"";
	}

	std::wstring result = path;
	CoTaskMemFree(path);
	psi->Release();
	pfd->Release();
	if(needUninit) CoUninitialize();

	return result;
}