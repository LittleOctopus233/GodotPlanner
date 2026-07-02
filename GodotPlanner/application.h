#pragma once

#include <string>
#include <vector>

#include "engine.h"
#include "project.h"

bool LoadEngines(std::vector<GodotEngine>& engines);

bool LoadProjects(std::vector<GodotProject>& projects);

bool OpenProject(const std::vector<GodotEngine>& engines, const std::wstring& projectPath);

const GodotEngine* SelectBestEngine(const std::vector<GodotEngine>& engines, const std::wstring& projectVersion);