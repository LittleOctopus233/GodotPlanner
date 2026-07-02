#include "application.h"

#include <windows.h>
#include <string>
#include <filesystem>
#include <fstream>

#include "IniReader.h"
#include "utility.h"
#include "file.h"

namespace fs = std::filesystem;

const std::wstring CONFIG_PATH = L"C:\\ProgramData\\GodotPlanner\\config.ini";

bool SaveConfig(const std::wstring& enginePath, const std::wstring& projectPath) {
	std::ofstream file(CONFIG_PATH);
	if(!file.is_open()) {
		MessageBoxW(NULL, L"写入配置文件失败！", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}

	file << "[path]\n";
	file << "engine_path=" << WideToUtf8(enginePath) << "\n";
	file << "project_path=" << WideToUtf8(projectPath) << "\n";
	file.close();

	return true;
}

bool EnsureConfigExists() {
	std::error_code ec;
	if(!fs::exists(CONFIG_PATH, ec)) {
		if(!CreateFileWithAutoDir(CONFIG_PATH)) {
			MessageBoxW(NULL, L"创建配置文件失败！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		std::wstring enginePath = SelectFolder(NULL, L"请选择Godot引擎所在目录");
		if(enginePath.empty()) {
			MessageBoxW(NULL, L"未选择引擎目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		std::wstring projectPath = SelectFolder(NULL, L"请选择项目所在目录");
		if(projectPath.empty()) {
			MessageBoxW(NULL, L"未选择项目目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		return SaveConfig(enginePath, projectPath);
	}

	IniReader reader;
	if(!reader.load(CONFIG_PATH)) {
		if(MessageBoxW(NULL, L"配置文件读取失败，是否重新配置？", L"提示", MB_YESNO | MB_ICONQUESTION) != IDYES) {
			return false;
		}

		std::wstring enginePath = SelectFolder(NULL, L"请选择Godot引擎所在目录");
		if(enginePath.empty()) {
			MessageBoxW(NULL, L"未选择引擎目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		std::wstring projectPath = SelectFolder(NULL, L"请选择项目所在目录");
		if(projectPath.empty()) {
			MessageBoxW(NULL, L"未选择项目目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		return SaveConfig(enginePath, projectPath);
	}

	std::string engine_path = reader.getString("path", "engine_path");
	std::string project_path = reader.getString("path", "project_path");

	if(engine_path.empty() || project_path.empty()) {
		if(MessageBoxW(NULL, L"配置文件内容不完整，是否重新配置？", L"提示", MB_YESNO | MB_ICONQUESTION) != IDYES) {
			return false;
		}

		std::wstring enginePath = SelectFolder(NULL, L"请选择Godot引擎所在目录");
		if(enginePath.empty()) {
			MessageBoxW(NULL, L"未选择引擎目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		std::wstring projectPath = SelectFolder(NULL, L"请选择项目所在目录");
		if(projectPath.empty()) {
			MessageBoxW(NULL, L"未选择项目目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		return SaveConfig(enginePath, projectPath);
	}

	return true;
}

bool LoadEngines(std::vector<GodotEngine>& engines) {
	if(!EnsureConfigExists()) {
		return false;
	}

	IniReader basic_reader;
	if(!basic_reader.load(CONFIG_PATH)) {
		MessageBoxW(NULL, L"基础配置读取失败！", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}

	std::string engine_path = basic_reader.getString("path", "engine_path");
	std::wstring wEnginePath = Utf8ToWide(engine_path);

	while(!IsDirectoryExists(wEnginePath)) {
		std::wstring msg = L"引擎路径不存在：" + wEnginePath + L"\n是否重新选择？";
		if(MessageBoxW(NULL, msg.c_str(), L"错误", MB_YESNO | MB_ICONERROR) != IDYES) {
			return false;
		}

		wEnginePath = SelectFolder(NULL, L"请选择Godot引擎所在目录");
		if(wEnginePath.empty()) {
			MessageBoxW(NULL, L"未选择引擎目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		std::string project_path = basic_reader.getString("path", "project_path");
		SaveConfig(wEnginePath, Utf8ToWide(project_path));
	}

	std::vector<FileInfo> fileList;
	EnumDirectoryFS(wEnginePath, fileList, true);

	for(const auto& item : fileList) {
		if(item.isDir) continue;
		GodotEngine engine;
		if(engine.ParseFromFileName(item.fileName, item.filePath)) {
			engines.push_back(engine);
		}
	}

	if(engines.empty()) {
		MessageBoxW(NULL, L"未找到任何Godot引擎！", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

bool LoadProjects(std::vector<GodotProject>& projects) {
	if(!EnsureConfigExists()) {
		return false;
	}

	IniReader basic_reader;
	if(!basic_reader.load(CONFIG_PATH)) {
		MessageBoxW(NULL, L"基础配置读取失败！", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}

	std::string project_path = basic_reader.getString("path", "project_path");
	std::wstring wProjectPath = Utf8ToWide(project_path);

	while(!IsDirectoryExists(wProjectPath)) {
		std::wstring msg = L"项目路径不存在：" + wProjectPath + L"\n是否重新选择？";
		if(MessageBoxW(NULL, msg.c_str(), L"错误", MB_YESNO | MB_ICONERROR) != IDYES) {
			return false;
		}

		wProjectPath = SelectFolder(NULL, L"请选择项目所在目录");
		if(wProjectPath.empty()) {
			MessageBoxW(NULL, L"未选择项目目录！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}

		std::string engine_path = basic_reader.getString("path", "engine_path");
		SaveConfig(Utf8ToWide(engine_path), wProjectPath);
	}

	std::vector<FileInfo> dirList;
	EnumDirectoryFS(wProjectPath, dirList, false);

	for(const auto& item : dirList) {
		if(!item.isDir) continue;
		std::wstring projectFilePath = item.filePath + L"\\project.godot";
		std::error_code ec;
		if(!fs::exists(projectFilePath, ec)) continue;

		GodotProject project;
		if(project.Load(projectFilePath, true)) {
			projects.push_back(project);
		}
	}

	return true;
}

const GodotEngine* SelectBestEngine(const std::vector<GodotEngine>& engines, const std::wstring& projectVersion) {
	size_t dot = projectVersion.find(L'.');
	if(dot == std::wstring::npos) return nullptr;

	int projMajor = std::stoi(projectVersion.substr(0, dot));
	int projMinor = std::stoi(projectVersion.substr(dot + 1));

	const GodotEngine* best = nullptr;
	for(const auto& e : engines) {
		if(!e.IsVersionMatch(projMajor, projMinor)) continue;
		if(!best || GodotEngine::IsBetterEngine(e, *best)) {
			best = &e;
		}
	}
	return best;
}

bool OpenProject(const std::vector<GodotEngine>& engines, const std::wstring& projectPath) {
	GodotProject project;
	if(!project.Load(projectPath)) {
		return false;
	}

	std::wstring wVersion = project.GetVersion();

	const GodotEngine* bestEngine = SelectBestEngine(engines, wVersion);
	if(bestEngine) {
		if(bestEngine->Launch(projectPath)) {
			return true;
		}
		else {
			MessageBoxW(NULL, L"启动引擎失败！", L"错误", MB_OK | MB_ICONERROR);
			return false;
		}
	}
	else {
		std::wstring msg = L"未找到与项目版本 " + wVersion + L" 兼容的 Godot 引擎。";
		MessageBoxW(NULL, msg.c_str(), L"错误", MB_OK | MB_ICONERROR);
		return false;
	}
}