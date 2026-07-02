#pragma once

#include <string>

class GodotProject {
private:
	std::wstring projectPath;
	std::wstring version;
	std::wstring projectName;
	int configVersion;

	std::string get_version(const std::string& str);

public:
	bool Load(const std::wstring& path, bool quiet = false);

	const std::wstring& GetVersion() const;

	const std::wstring& GetProjectPath() const;

	const std::wstring& GetProjectName() const;
};