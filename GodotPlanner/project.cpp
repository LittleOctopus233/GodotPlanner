#include "project.h"

#include <windows.h>
#include <string>

#include "IniReader.h"
#include "utility.h"

std::string GodotProject::get_version(const std::string& str) {
	size_t first = str.find('"');
	if(first == std::string::npos) return "";
	size_t second = str.find('"', first + 1);
	if(second == std::string::npos) return "";
	return str.substr(first + 1, second - first - 1);
}

bool GodotProject::Load(const std::wstring& path, bool quiet) {
	IniReader reader;
	if(!reader.load(path)) {
		if(!quiet) MessageBoxW(NULL, L"项目配置读取失败！", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}

	configVersion = reader.getInt("", "config_version");
	if(configVersion != 5) {
		if(!quiet) MessageBoxW(NULL, L"项目文件版本不兼容！", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}

	std::string features = reader.getString("application", "config/features");
	if(features.empty()) {
		if(!quiet) MessageBoxW(NULL, L"项目文件缺少版本配置！", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}

	std::string versionStr = get_version(features);
	version = Utf8ToWide(versionStr);
	projectPath = path;

	std::string nameStr = reader.getString("application", "config/name");
	if(!nameStr.empty()) {
		projectName = Utf8ToWide(get_version(nameStr));
	}
	else {
		projectName = L"未命名项目";
	}

	return true;
}

const std::wstring& GodotProject::GetVersion() const {
	return version;
}

const std::wstring& GodotProject::GetProjectPath() const {
	return projectPath;
}

const std::wstring& GodotProject::GetProjectName() const {
	return projectName;
}