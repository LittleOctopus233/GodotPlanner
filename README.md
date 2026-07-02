# GodotPlanner

Godot引擎和项目管理工具，帮助开发者快速管理多个Godot引擎版本和项目。GUI版本还没做，目前只能通过命令行方式使用。

### 前言:该项目大量使用ai生成的代码，遇见史请不要慌张，也欢迎大佬来修改。别问为什么不用git，问就是懒得下。

## 功能特性

- **引擎管理**：自动扫描并管理多个Godot引擎版本
- **项目管理**：自动识别项目并关联对应Godot版本
- **智能匹配**：根据项目版本自动选择最佳兼容引擎
- **快速启动**：一键启动引擎或打开项目
- **命令行支持**：支持通过项目文件路径直接打开项目

## 技术栈

- **语言**：C++17
- **平台**：Windows
- **UI**：Windows Console + Win32 API

## 编译环境

- Visual Studio 2022
- Windows SDK 10.0.22621.0+

## 使用方法

### 首次运行

1. 启动程序后，会自动检测配置文件是否存在
2. 如果配置文件不存在，会引导您选择：
   - Godot引擎所在目录
   - 项目所在目录
3. 配置完成后，程序会自动保存配置到 `C:\ProgramData\GodotPlanner\config.ini`

### 主菜单操作

1. **打开引擎**：选择一个已安装的Godot引擎启动项目管理器
2. **打开项目**：选择一个项目，程序会自动匹配最佳引擎并打开
3. **退出**：退出程序

### 命令行方式
(可手动设置.godot文件打开方式为此应用，即可双击.godot文件打开项目)

```bash
GodotPlanner.exe "C:\path\to\project\project.godot"
```

通过命令行传入项目文件路径，程序会静默加载并直接打开项目，不会显示控制台窗口。

## 配置文件

配置文件位于 `C:\ProgramData\GodotPlanner\config.ini`

```ini
[path]
engine_path=C:\Godot\Engines
project_path=C:\Godot\Projects
```

- `engine_path`：Godot引擎所在目录，程序会递归扫描该目录下的Godot可执行文件
- `project_path`：项目所在目录，程序会扫描该目录下所有包含 `project.godot` 的子目录

## 引擎命名规则

程序通过文件名识别Godot引擎，支持以下格式(即官方命名规则)：

```
Godot_vX.Y_win64.exe
Godot_vX.Y-stable_win64.exe
Godot_vX.Y.Z-stable_win64.exe
Godot_vX.Y-beta_win64.exe
Godot_vX.Y-beta1_win64.exe
Godot_vX.Y-rc_win64.exe
Godot_vX.Y-dev_win64.exe
```

## 项目文件格式

程序通过读取 `project.godot` 文件识别项目：

- 读取 `config_version` 字段验证文件格式
- 读取 `[application]` 下的 `config/features` 获取项目版本号
- 读取 `[application]` 下的 `config/name` 获取项目名称

## 版本匹配逻辑

1. 优先匹配主版本号和次版本号
2. 在同版本中，优先选择stable版本
3. 同类型版本中，选择最高补丁版本
4. 预发布版本优先级：rc > beta > dev

## 注意事项

- 程序以独立进程方式启动Godot引擎，启动后本程序会退出
- 首次运行需要管理员权限以创建配置目录
- 配置文件中的路径需要使用正斜杠 `/` 或双反斜杠 `\\`

## 许可证

MIT License