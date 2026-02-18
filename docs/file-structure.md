# 文件组织结构文档

> [!note]
> 本文由AI生成，可能存在错误或不完整的部分。

本文档描述了 DesktopDanmaku 项目的文件组织结构，帮助开发者了解项目的整体架构和各文件的功能。

## 项目根目录

```
DesktopDanmaku/
├── .vscode/            # VS Code 配置文件
├── docs/              # 项目文档
├── include/           # 头文件目录
├── src/               # 源文件目录
├── .gitignore         # Git 忽略文件配置
├── LICENSE            # 许可证文件
├── README.MD          # 项目说明文件
├── installer.iss      # Inno Setup 安装脚本
└── makefile           # 编译配置文件
```

## 目录详解

### 1. .vscode/

VS Code 编辑器的配置文件目录，包含项目特定的编辑器设置。

### 2. docs/

项目文档目录，存放各种项目相关文档。

- **makefileHelper.txt** - makefile 辅助说明文件
- **file-structure.md** - 本文件，项目文件组织结构文档

### 3. include/

头文件目录，存放项目的所有头文件，按照功能模块分类。

#### 3.1 include/danmaku/

弹幕相关的头文件。

- **dmkitem.hpp** - 弹幕项的定义和相关操作

#### 3.2 include/functions/

通用功能函数的头文件。

- **files.hpp** - 文件操作相关函数
- **others.hpp** - 其他辅助函数
- **randnum.hpp** - 随机数生成相关函数

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

### 4. src/

源文件目录，存放项目的所有源文件，与 include/ 目录结构对应。

#### 4.1 src/danmaku/

弹幕相关的源文件。

- **dmkitem.cpp** - 弹幕项的实现

#### 4.2 src/functions/

通用功能函数的源文件。

- **files.cpp** - 文件操作相关函数实现
- **others.cpp** - 其他辅助函数实现
- **randnum.cpp** - 随机数生成相关函数实现

#### 4.3 src/windows/

窗口相关的源文件。

- **elementID.cpp** - 元素 ID 管理实现
- **elements.cpp** - 窗口元素实现
- **main.cpp** - 主窗口实现
- **overlay.cpp** - 覆盖层窗口实现

#### 4.4 根源文件

- **debug.cpp** - 调试相关实现
- **desktopDanmaku.exe.manifest** - 应用程序清单文件
- **list.rc** - 资源文件
- **main.cpp** - 主程序入口实现

## 文件功能说明

### 核心文件

| 文件路径 | 功能说明 |
|---------|--------|
| src/main.cpp | 主程序入口，负责初始化和启动应用 |
| include/main.hpp | 主程序入口相关定义 |
| include/debug.hpp | 调试相关定义和宏 |
| src/debug.cpp | 调试功能实现 |

### 弹幕系统

| 文件路径 | 功能说明 |
|---------|--------|
| include/danmaku/dmkitem.hpp | 弹幕项的定义和相关操作 |
| src/danmaku/dmkitem.cpp | 弹幕项的实现 |

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
| include/functions/others.hpp | 其他辅助函数 |
| include/functions/randnum.hpp | 随机数生成相关函数 |
| src/functions/files.cpp | 文件操作相关函数实现 |
| src/functions/others.cpp | 其他辅助函数实现 |
| src/functions/randnum.cpp | 随机数生成相关函数实现 |

### 配置和构建文件

| 文件路径 | 功能说明 |
|---------|--------|
| makefile | 编译配置文件，定义编译规则和目标 |
| installer.iss | Inno Setup 安装脚本，用于生成安装包 |
| .gitignore | Git 忽略文件配置，指定不需要版本控制的文件 |
| LICENSE | 许可证文件，定义项目的开源协议 |
| README.MD | 项目说明文件，包含项目介绍、功能列表和构建说明 |

## 代码组织结构

项目采用模块化设计，代码组织结构清晰：

1. **分层结构**：将头文件和源文件分离，分别存放在 include/ 和 src/ 目录
2. **功能模块化**：按照功能将代码分为多个模块，如弹幕模块、窗口模块、工具函数模块等
3. **命名规范**：文件和目录命名清晰，反映其功能和用途

## 开发建议

1. **新增文件**：当添加新功能时，应按照现有目录结构，将新文件放在相应的目录中
2. **代码风格**：保持与现有代码风格一致，确保代码可读性
3. **文档更新**：当修改文件结构或添加新文件时，应及时更新本文档

## 项目技术栈

- **开发语言**：C++17
- **平台**：Windows
- **技术**：win32 API, GDI+
- **构建工具**：GNU Make
- **开发环境**：MSYS2 (MingGW64), VS Code

---

本文档旨在帮助开发者快速了解项目的文件组织结构，如有任何疑问或建议，请联系项目维护者。