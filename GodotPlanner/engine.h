#pragma once

#include <string>

class GodotEngine {
private:
	std::wstring fullPath;
	std::wstring versionCore;
	int major;
	int minor;
	int patch;
	std::wstring releaseType;
	int releaseNumber;
	std::wstring displayName;

public:
	GodotEngine() : major(0), minor(0), patch(-1), releaseNumber(0) {}

	bool ParseFromFileName(const std::wstring& fileName, const std::wstring& filePath);

	static bool IsBetterEngine(const GodotEngine& a, const GodotEngine& b);

	bool Launch(const std::wstring& projectPath) const;

bool LaunchProjectManager() const;

	const std::wstring& GetFullPath() const;

	const std::wstring& GetVersionCore() const;

	const std::wstring& GetDisplayName() const;

	int GetMajor() const;

	int GetMinor() const;

	bool IsVersionMatch(int projMajor, int projMinor) const;
};