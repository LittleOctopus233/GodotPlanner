#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>

#include "application.h"
#include "utility.h"

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:wmainCRTStartup")

void ShowBasicInfo() {
	WriteLineW(L"================================");
	WriteLineW(L"Godot统筹 v0.3.1-console");
	WriteLineW(L"");
	WriteLineW(L"made by : OctopusZhang");
	WriteLineW(L"made date : 2026/7/13");
	WriteLineW(L"");
	WriteLineW(L"纳西妲世界第一可爱");
	WriteLineW(L"================================");
	WriteLineW(L"");
}

void ShowMenu() {
	WriteLineW(L"================================");
	WriteLineW(L"    Godot统筹 v0.3.1-console");
	WriteLineW(L"================================");
	WriteLineW(L"(a) 打开引擎      (b) 打开项目");
	WriteLineW(L"================================");
}

void ShowDownMenu() {
	WriteLineW(L"================================");
	WriteLineW(L"(q) 上一页        (e) 下一页  ");
	WriteLineW(L"================================");
	WriteLineW(L"(c) 退出程序      (d) 关于项目");
	WriteLineW(L"================================");
}

void ShowSelectEngineInfo(const std::vector<GodotEngine>& engines, int page = 1) {
	if(engines.empty()) {
		MessageBoxW(NULL, L"未找到任何Godot引擎！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	WriteLineW(L"请选择引擎:");
	for(size_t i = (page - 1) * 10; i < engines.size() && i < page * 10; ++i) {
		WriteLineW(L"(" + std::to_wstring(i + 1 - (page - 1) * 10) + L"). " + engines[i].GetDisplayName());
	}
}

void ShowSelectProjectInfo(const std::vector<GodotProject>& projects, int page = 1) {
	if(projects.empty()) {
		MessageBoxW(NULL, L"未找到任何Godot项目！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	WriteLineW(L"请选择项目:");
	for(size_t i = (page - 1) * 10; i < projects.size() && i < page * 10; ++i) {
		WriteLineW(L"(" + std::to_wstring(i + 1 - (page - 1) * 10) + L"). " + projects[i].GetProjectName() + L" [Godot " + projects[i].GetVersion() + L"]");
	}
}

void SelectEngine(const std::vector<GodotEngine>& engines, int choice, int page = 1) {
	if (choice < 1 || choice > 10 || choice > engines.size() - (page - 1) * 10) {
		MessageBoxW(NULL, L"无效选项！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	WriteLineW(L"开始启动引擎: " + engines[(page - 1) * 10 + choice - 1].GetDisplayName());

	const GodotEngine& engine = engines[(page - 1) * 10 + choice - 1];
	if (engine.LaunchProjectManager()) {
		exit(0);
	}
	else {
		MessageBoxW(NULL, L"引擎启动失败！", L"错误", MB_OK | MB_ICONERROR);
	}
}

void SelectProject(const std::vector<GodotEngine>& engines, const std::vector<GodotProject>& projects, int choice, int page = 1) {
	if(choice < 1 || choice > 10 || choice > projects.size() - (page - 1) * 10) {
		MessageBoxW(NULL, L"无效选项！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	WriteLineW(L"开始启动项目: " + projects[(page - 1) * 10 + choice - 1].GetProjectName());

	const GodotProject& project = projects[(page - 1) * 10 + choice - 1];
	if(OpenProject(engines, project.GetProjectPath())) {
		exit(0);
	}
	else {
		MessageBoxW(NULL, L"项目启动失败！", L"错误", MB_OK | MB_ICONERROR);
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

		if(!LoadEngines(engines)) {
			return 1;
		}

		if(!LoadProjects(projects)) {
			return 1;
		}

		int mode = 1;// 1: engine mode, 2: project mode
		int page = 1;

		while(true) {
			system("cls");
			ShowMenu();

			if (mode == 1) {
				ShowSelectEngineInfo(engines, page);
			}
			else if (mode == 2) {
				ShowSelectProjectInfo(projects, page);
			}

			ShowDownMenu();

			int choice = _getch();

			switch(choice) {
			case 'a':
				mode = 1;
				page = 1;
				break;
			case 'b':
				mode = 2;
				page = 1;
				break;
			case 'c':
				return 0;
			case 'd':
				system("cls");
				ShowBasicInfo();
				WriteLineW(L"按任意键返回菜单...");
				_getch();
				break;
			case 'q':
				if (page > 1) {
					page--;
				}
				break;
			case 'e':
				if (mode == 1 && page * 10 < engines.size()) {
					page++;
				}
				else if(mode == 2 && page * 10 < projects.size()) {
					page++;
				}
				break;
			default:
				if (choice == '0') choice += 10;
				if (mode == 1) {
					SelectEngine(engines, choice - '0', page);
				}
				else if (mode == 2) {
					SelectProject(engines, projects, choice - '0', page);
				}
				break;
			}
		}
	}
	return 0;
}
