# 文件组织结构文档

> [!note]
> 本文由 AI 生成并定期更新，如有不准确的地方欢迎反馈和修正。

本文档描述了 DesktopDanmaku 项目的文件组织结构，帮助开发者了解项目的整体架构和各文件的功能。

## 项目根目录

```
DesktopDanmaku/
├── .vscode/               # VS Code 配置文件（仅 launch.json 和 tasks.json 被版本控制）
├── docs/                  # 项目文档
├── include/               # 头文件目录
├── resource/              # 资源文件目录（图标、截图等）
├── src/                   # 源文件目录
├── .gitignore             # Git 忽略文件配置
├── LICENSE                # 许可证文件
├── README.MD              # 项目说明文件
├── installer.iss          # Inno Setup 安装脚本
├── makefile               # 编译配置文件
└── build/                 # 编译输出目录（本地）
```

## 目录详解

### 1. .vscode/

VS Code 编辑器的配置文件目录，包含项目特定的编辑器设置。

- **.vscode/launch.json** - 调试配置
- **.vscode/tasks.json** - 任务配置

### 2. docs/

项目文档目录，存放各种项目相关文档。

- **file-structure.md** - 本文件，项目文件组织结构文档
- **makefileHelper.txt** - makefile 辅助说明文件

### 3. include/

头文件目录，存放项目的所有头文件，按照功能模块分类。

#### 3.1 include/danmaku/

弹幕相关的头文件。

- **dmkitem.hpp** - 弹幕项的定义和相关操作
- **dmkbmpcache.hpp** - 弹幕位图缓存管理
- **dmkmgr.hpp** - 弹幕管理器定义

#### 3.2 include/functions/

通用功能函数的头文件。

- **files.hpp** - 文件操作相关函数
- **gpptr.hpp** - GDI+ 指针智能指针包装
- **jsonReader.hpp** - JSON 文件读取相关函数
- **others.hpp** - 其他辅助函数
- **randnum.hpp** - 随机数生成相关函数
- **srwlk.hpp** - 线程安全的读写锁实现
- **str.hpp** - 字符串处理相关函数

#### 3.3 include/windows/

窗口相关的头文件。

- **base.hpp** - 窗口基础定义
- **elements.hpp** - 窗口元素定义
- **extraElementInfo.hpp** - 额外元素信息定义
- **font.hpp** - 字体相关定义
- **main.hpp** - 主窗口定义
- **overlay.hpp** - 覆盖层窗口定义

#### 3.4 根头文件

- **debug.hpp** - 调试相关定义
- **main.hpp** - 主程序入口相关定义
- **pch.hpp** - 预编译头文件（包含常用的标准库和 Windows 头文件）

### 4. resource/

资源文件目录，存放应用程序的资源文件。

#### 4.1 resource/icon/

应用程序图标文件。

- 各种尺寸的应用程序图标

#### 4.2 resource/for_readme/

README 文档所用的资源文件。

- **application-screenshoot.png** - 应用程序截图

#### 4.3 根资源文件

- **appInfo.rc** - 应用程序版本信息和资源文件

### 5. src/

源文件目录，存放项目的所有源文件，与 include/ 目录结构对应。

#### 5.1 src/danmaku/

弹幕相关的源文件。

- **dmkitem.cpp** - 弹幕项的实现
- **dmkbmpcache.cpp** - 弹幕位图缓存的实现
- **dmkmgr.cpp** - 弹幕管理器的实现

#### 5.2 src/functions/

通用功能函数的源文件。

- **files.cpp** - 文件操作相关函数实现
- **jsonReader.cpp** - JSON 文件读取实现
- **others.cpp** - 其他辅助函数实现
- **randnum.cpp** - 随机数生成相关函数实现
- **str.cpp** - 字符串处理相关函数实现

#### 5.3 src/windows/

窗口相关的源文件。

- **elementID.cpp** - 元素 ID 管理实现
- **elements.cpp** - 窗口元素实现
- **main.cpp** - 主窗口实现
- **overlay.cpp** - 覆盖层窗口实现

#### 5.4 根源文件

- **debug.cpp** - 调试相关实现
- **desktopDanmaku.exe.manifest** - 应用程序清单文件（Windows DPI 感知等设置）
- **list.rc** - 资源脚本文件
- **main.cpp** - 主程序入口实现

## 文件功能说明

### 核心文件

| 文件路径 | 功能说明 |
|---------|--------|
| src/main.cpp | 主程序入口，负责初始化应用和消息循环 |
| include/main.hpp | 主程序入口相关定义 |
| include/debug.hpp | 调试相关定义和宏 |
| src/debug.cpp | 调试功能实现 |
| include/pch.hpp | 预编译头文件，包含常用依赖 |

### 弹幕系统

| 文件路径 | 功能说明 |
|---------|--------|
| include/danmaku/dmkitem.hpp | 弹幕项的数据结构和操作接口 |
| src/danmaku/dmkitem.cpp | 弹幕项的实现 |
| include/danmaku/dmkbmpcache.hpp | 弹幕位图缓存的定义 |
| src/danmaku/dmkbmpcache.cpp | 弹幕位图缓存的实现（用于性能优化） |
| include/danmaku/dmkmgr.hpp | 弹幕管理器定义（管理所有弹幕的生命周期） |
| src/danmaku/dmkmgr.cpp | 弹幕管理器的实现 |

### 窗口系统

| 文件路径 | 功能说明 |
|---------|--------|
| include/windows/base.hpp | 窗口基础定义和通用窗口操作 |
| include/windows/main.hpp | 主窗口定义 |
| include/windows/overlay.hpp | 覆盖层窗口定义（用于显示弹幕） |
| include/windows/elements.hpp | 窗口元素定义（如按钮、文本框等） |
| include/windows/extraElementInfo.hpp | 额外元素信息定义 |
| include/windows/font.hpp | 字体相关定义和操作 |
| src/windows/main.cpp | 主窗口实现 |
| src/windows/overlay.cpp | 覆盖层窗口实现 |
| src/windows/elements.cpp | 窗口元素实现 |
| src/windows/elementID.cpp | 元素 ID 管理实现 |

### 工具函数

| 文件路径 | 功能说明 |
|---------|--------|
| include/functions/files.hpp | 文件操作相关函数（如读取配置文件） |
| src/functions/files.cpp | 文件操作函数实现 |
| include/functions/gpptr.hpp | GDI+ 智能指针包装（RAII 模式） |
| include/functions/jsonReader.hpp | JSON 文件读取接口 |
| src/functions/jsonReader.cpp | JSON 文件读取实现（基于第三方库） |
| include/functions/others.hpp | 其他辅助函数 |
| src/functions/others.cpp | 其他辅助函数实现 |
| include/functions/randnum.hpp | 随机数生成相关函数 |
| src/functions/randnum.cpp | 随机数生成函数实现 |
| include/functions/srwlk.hpp | 线程安全的读写锁（用于多线程场景） |
| include/functions/str.hpp | 字符串处理相关函数 |
| src/functions/str.cpp | 字符串处理函数实现 |

### 资源文件

| 文件路径 | 功能说明 |
|---------|--------|
| resource/appInfo.rc | 应用程序版本号、公司信息、产品描述等元数据 |
| resource/icon/ | 应用程序图标资源 |
| resource/for_readme/ | README 文档所用的截图和说明图片 |
| src/desktopDanmaku.exe.manifest | 应用程序清单（Windows DPI 感知、管理员权限等） |
| src/list.rc | 资源脚本文件 |

### 配置和构建文件

| 文件路径 | 功能说明 |
|---------|--------|
| makefile | 编译配置文件，定义编译规则和目标 |
| installer.iss | Inno Setup 安装脚本，用于生成 Windows 安装包 |
| .gitignore | Git 忽略文件配置，指定不需要版本控制的文件 |
| LICENSE | 许可证文件，定义项目的开源协议 |
| README.MD | 项目说明文件，包含项目介绍、功能列表和构建说明 |

## 代码组织结构

项目采用模块化设计，代码组织结构清晰：

1. **分层结构**：将头文件和源文件分离，分别存放在 include/ 和 src/ 目录
2. **功能模块化**：按照功能将代码分为多个模块
   - **弹幕模块（danmaku）** - 处理弹幕数据和缓存
   - **窗口模块（windows）** - 处理 UI 和窗口管理
   - **工具函数模块（functions）** - 通用辅助函数
3. **命名规范**：文件和目录命名清晰，反映其功能和用途
4. **资源管理**：使用 RAII 模式（如 gpptr 智能指针）管理系统资源
5. **线程安全**：使用读写锁（srwlk）保证多线程场景下的数据安全

## 开发建议

1. **新增功能**：当添加新功能时，应按照现有目录结构，将新文件放在相应的目录中
   - 新增弹幕相关功能 → `include/danmaku/` 和 `src/danmaku/`
   - 新增窗口相关功能 → `include/windows/` 和 `src/windows/`
   - 新增工具函数 → `include/functions/` 和 `src/functions/`

2. **代码风格**：保持与现有代码风格一致，确保代码可读性
   - 头文件使用 `.hpp` 后缀
   - 源文件使用 `.cpp` 后缀
   - 类名采用 CamelCase，函数名采用 camelCase

3. **资源管理**：
   - 优先使用智能指针（如 gpptr）而非裸指针
   - 在多线程场景使用读写锁确保线程安全

4. **文档更新**：当修改文件结构或添加新文件时，应及时更新本文档

## 项目��术栈

- **开发语言**：C++17
- **平台**：Windows 10+
- **图形库**：GDI+
- **API**：Win32 API
- **JSON 库**：第三方 JSON 库（见 jsonReader.cpp）
- **构建工具**：GNU Make
- **开发环境**：
  - 操作系统：Windows 10 x64
  - 编译器：MinGW64（MSYS2）
  - 编辑器：VS Code
  - GNU Make：4.4.1+
  - 打包工具：Inno Setup

## 编译和构建

更多关于编译和构建的信息，请查看 README.MD 文件中的"构建方式"部分。

---

本文档旨在帮助开发者快速了解项目的文件组织结构，如有任何疑问或发现不准确之处，欢迎反馈和修正。
