#include <regex>
#include <windows.h>
#include <string>

#include "engine.h"

bool GodotEngine::IsBetterEngine(const GodotEngine& a, const GodotEngine& b) {
	auto compareVersion = [](int a1, int b1, int a2, int b2, int a3, int b3) -> int {
		if(a1 != b1) return a1 > b1 ? 1 : -1;
		if(a2 != b2) return a2 > b2 ? 1 : -1;
		if(a3 != b3) return a3 > b3 ? 1 : -1;
		return 0;
		};

	bool aStable = (a.releaseType == L"stable");
	bool bStable = (b.releaseType == L"stable");

	if(aStable && !bStable) return true;
	if(!aStable && bStable) return false;

	int cmp = compareVersion(a.major, b.major, a.minor, b.minor, a.patch, b.patch);
	if(cmp != 0) return cmp > 0;

	if(!aStable) {
		auto typeRank = [](const std::wstring& t) {
			if(t == L"rc") return 3;
			if(t == L"beta") return 2;
			if(t == L"dev") return 1;
			return 0;
			};
		int ra = typeRank(a.releaseType);
		int rb = typeRank(b.releaseType);
		if(ra != rb) return ra > rb;
		return a.releaseNumber > b.releaseNumber;
	}
	return false;
}

bool GodotEngine::ParseFromFileName(const std::wstring& fileName, const std::wstring& filePath) {
	std::wregex pattern(
		LR"(^Godot_v(\d+)\.(\d+)(\.(\d+))?(-(stable|beta|dev|rc)(\d*))?_win64\.exe$)"
	);

	std::wsmatch match;
	if(!std::regex_match(fileName, match, pattern)) {
		return false;
	}

	fullPath = filePath;
	major = std::stoi(match[1].str());
	minor = std::stoi(match[2].str());

	if(match[4].matched) {
		patch = std::stoi(match[4].str());
		versionCore = match[1].str() + L"." + match[2].str() + L"." + match[4].str();
	}
	else {
		patch = -1;
		versionCore = match[1].str() + L"." + match[2].str();
	}

	if(match[6].matched) {
		releaseType = match[6].str();
		std::wstring numStr = match[7].str();
		releaseNumber = numStr.empty() ? 0 : std::stoi(numStr);
		displayName = versionCore + L"-" + releaseType + (numStr.empty() ? L"" : numStr);
	}
	else {
		releaseType = L"stable";
		releaseNumber = 0;
		displayName = versionCore + L"-stable";
	}
	return true;
}

bool GodotEngine::Launch(const std::wstring& projectPath) const {
	std::wstring wCmd = L"\"" + fullPath + L"\" --editor \"" + projectPath + L"\"";

	STARTUPINFOW si = {sizeof(si)};
	PROCESS_INFORMATION pi = {0};
	if(CreateProcessW(NULL, &wCmd[0], NULL, NULL, FALSE,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	}
	else {
		return false;
	}
}

bool GodotEngine::LaunchProjectManager() const {
	std::wstring wCmd = L"\"" + fullPath + L"\" --project-manager";

	STARTUPINFOW si = {sizeof(si)};
	PROCESS_INFORMATION pi = {0};
	if(CreateProcessW(NULL, &wCmd[0], NULL, NULL, FALSE,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	}
	else {
		return false;
	}
}

const std::wstring& GodotEngine::GetFullPath() const {
	return fullPath;
}

const std::wstring& GodotEngine::GetVersionCore() const {
	return versionCore;
}

const std::wstring& GodotEngine::GetDisplayName() const {
	return displayName;
}

int GodotEngine::GetMajor() const {
	return major;
}

int GodotEngine::GetMinor() const {
	return minor;
}

bool GodotEngine::IsVersionMatch(int projMajor, int projMinor) const {
	return major == projMajor && minor == projMinor;
}