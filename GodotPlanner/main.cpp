#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>

#include "application.h"
#include "utility.h"

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:wmainCRTStartup")

void ShowMenu() {
	WriteLineW(L"");
	WriteLineW(L"================================");
	WriteLineW(L"请选择操作:");
	WriteLineW(L"1. 打开引擎");
	WriteLineW(L"2. 打开项目");
	WriteLineW(L"3. 退出");
	WriteLineW(L"================================");
	WriteW(L"输入选项(1-3): ");
}

void SelectEngine(const std::vector<GodotEngine>& engines) {
	if(engines.empty()) {
		MessageBoxW(NULL, L"未找到任何Godot引擎！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	WriteLineW(L"");
	WriteLineW(L"可用引擎列表:");
	for(size_t i = 0; i < engines.size(); ++i) {
		WriteLineW(std::to_wstring(i + 1) + L". " + engines[i].GetDisplayName());
	}

	WriteW(L"输入引擎序号: ");
	int choice = _getch() - '0';
	WriteLineW(std::to_wstring(choice));

	if(choice < 1 || static_cast<size_t>(choice) > engines.size()) {
		MessageBoxW(NULL, L"无效选项！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	const GodotEngine& engine = engines[choice - 1];
	if(engine.LaunchProjectManager()) {
		exit(0);
	}
	else {
		MessageBoxW(NULL, L"引擎启动失败！", L"错误", MB_OK | MB_ICONERROR);
	}
}

void SelectProject(const std::vector<GodotEngine>& engines, const std::vector<GodotProject>& projects) {
	if(projects.empty()) {
		MessageBoxW(NULL, L"未找到任何Godot项目！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	WriteLineW(L"");
	WriteLineW(L"可用项目列表:");
	for(size_t i = 0; i < projects.size(); ++i) {
		WriteLineW(std::to_wstring(i + 1) + L". " + projects[i].GetProjectName() + L" [Godot " + projects[i].GetVersion() + L"]");
	}

	WriteW(L"输入项目序号: ");
	int choice = _getch() - '0';
	WriteLineW(std::to_wstring(choice));

	if(choice < 1 || static_cast<size_t>(choice) > projects.size()) {
		MessageBoxW(NULL, L"无效选项！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	const GodotProject& project = projects[choice - 1];
	if(OpenProject(engines, project.GetProjectPath())) {
		exit(0);
	}
}

int wmain(int argc, wchar_t* argv[]) {
	std::vector<GodotEngine> engines;
	std::vector<GodotProject> projects;

	if(argc > 1) {
		std::wstring wProjectPath = argv[1];

		if(!LoadEngines(engines)) {
			return 1;
		}

		if(!OpenProject(engines, wProjectPath)) {
			return 1;
		}

		return 0;
	}
	else {
		ShowConsole();
		ShowBasicInfo();

		if(!LoadEngines(engines)) {
			return 1;
		}

		if(!LoadProjects(projects)) {
			return 1;
		}

		while(true) {
			ShowMenu();
			int choice = _getch() - '0';
			WriteLineW(std::to_wstring(choice));

			switch(choice) {
			case 1:
				SelectEngine(engines);
				break;
			case 2:
				SelectProject(engines, projects);
				break;
			case 3:
				WriteLineW(L"退出程序...");
				return 0;
			default:
				MessageBoxW(NULL, L"无效选项，请重新输入！", L"错误", MB_OK | MB_ICONERROR);
				break;
			}
		}
	}
	return 0;
}