# 构建指南

> [!note]
> 本文由AI生成，可能存在错误或不完整的部分。

本文档详细说明如何构建 DesktopDanmaku 项目，包括构建环境搭建、构建命令和构建过程中的常见问题。

## 1. 构建环境

### 1.1 必要工具

- **操作系统**：Windows 10 或更高版本
- **C++ 编译器**：支持 C++17 的编译器，可通过 g++ 命令调用（如 GCC、MinGW-w64 等）
- **GNU Make**：用于执行构建命令

### 1.2 推荐工具

- **MSYS2**：提供 MingGW64 编译环境（可选，方便获取 g++ 编译器）

### 1.3 环境搭建

#### 1.3.1 安装C++编译器

以下是在Windows系统上安装C++编译器的方法，确保可通过`g++`命令调用，你一共有两个选项：

##### 选项 1：使用 MSYS2（推荐，Windows 下最方便的方式）

1. 从 [MSYS2 官网](https://www.msys2.org/) 下载并安装 Windows 版本的 MSYS2
2. 安装完成后，打开 MSYS2 MSYS 终端
3. 运行以下命令更新系统包：

```bash
pacman -Syu
```

4. 重启终端，再次运行更新命令：

```bash
pacman -Syu
```

5. 安装 MingGW64 工具链（包含 g++ 编译器）：

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

6. 将 `C:\msys64\mingw64\bin` 添加到系统环境变量 `PATH` 中

##### 选项 2：直接安装 MinGW-w64

1. 从 [MinGW-w64 官网](https://mingw-w64.org/doku.php) 下载 Windows 版本的 MinGW-w64
2. 运行安装程序，选择合适的架构和版本
3. 将安装目录下的 `bin` 文件夹添加到系统环境变量 `PATH` 中

#### 1.3.2 安装 GNU Make

如果使用选项 2 安装 MinGW-w64 后，运行 `make --version` 显示找不到命令，则需要单独安装 GNU Make：

1. 从 [GNU Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm) 下载安装包
2. 运行安装程序
3. 将安装目录下的 `bin` 文件夹添加到系统环境变量 `PATH` 中

#### 1.3.3 验证环境安装

在命令提示符中运行以下命令，验证编译器是否安装成功：

```bash
g++ --version
make --version
```

## 2. 构建项目

### 2.1 基本构建命令

在项目根目录下运行以下命令：

#### 2.1.1 构建 DEBUG 版本（默认 64 位）

```bash
make
```

这将在 `build/64/debug` 目录下生成可执行文件 `danmaku.exe`。

#### 2.1.2 构建 32 位 DEBUG 版本

```bash
make ARCH=32
```

这将在 `build/32/debug` 目录下生成 32 位可执行文件 `danmaku.exe`。

#### 2.1.3 构建 RELEASE 版本（64 位）

```bash
make DEBUG=0
```

这将在 `build/64/release` 目录下生成优化后的可执行文件 `danmaku.exe`。

#### 2.1.4 构建 32 位 RELEASE 版本

```bash
make ARCH=32 DEBUG=0
```

这将在 `build/32/release` 目录下生成 32 位优化后的可执行文件 `danmaku.exe`。

### 2.2 构建选项

| 选项 | 描述 | 默认值 |
|------|------|--------|
| ARCH | 架构，支持 32 或 64 | 64 |
| DEBUG | 是否构建 debug 版本，1 为是，0 为否 | 1 |
| CXX | C++ 编译器 | 64位: g++，32位: i686-w64-mingw32-g++ |
| CXXFLAGS | C++ 编译器标志 | -std=c++17 -Wall -Wextra -Wpedantic |
| LDFLAGS | 链接器标志 | -mwindows -municode |
| LDLIBS | 链接库 | -luser32 -lgdi32 -lcomctl32 -lgdiplus |

### 2.3 其他构建目标

#### 2.3.1 查看帮助信息

```bash
make help
```

显示 makefile 帮助信息（实际会显示 docs/makefileHelper.txt 文件内容）。

#### 2.3.2 清理构建文件

```bash
make clean
```

删除所有构建生成的文件和目录（整个 build 目录）。

#### 2.3.3 运行编译结果

```bash
make run
```

编译并运行程序（默认运行 64 位版本）。

#### 2.3.4 构建所有发布版本

```bash
make release
```

清理并构建 64 位和 32 位发布版本。

#### 2.3.5 构建 64 位发布版本

```bash
make release64
```

构建 64 位发布版本。

#### 2.3.6 构建 32 位发布版本

```bash
make release32
```

构建 32 位发布版本。

#### 2.3.7 构建安装包

```bash
make installer
```

清理并构建 64 位和 32 位安装包。

#### 2.3.8 构建 64 位安装包

```bash
make installer64
```

构建 64 位安装包。

#### 2.3.9 构建 32 位安装包

```bash
make installer32
```

构建 32 位安装包。

## 3. 构建过程详解

### 3.1 构建目录结构

构建过程会在项目根目录下创建 `build` 目录，并根据架构和构建类型创建子目录：

```
build/
├── 64/
│   ├── debug/       # 64 位 debug 版本
│   │   ├── obj/     # 目标文件目录
│   │   └── danmaku.exe  # 可执行文件
│   └── release/     # 64 位 release 版本
│       ├── obj/     # 目标文件目录
│       └── danmaku.exe  # 可执行文件
└── 32/
    ├── debug/       # 32 位 debug 版本
    │   ├── obj/     # 目标文件目录
    │   └── danmaku.exe  # 可执行文件
    └── release/     # 32 位 release 版本
        ├── obj/     # 目标文件目录
        └── danmaku.exe  # 可执行文件
```

### 3.2 构建流程

1. 检查构建目录是否存在，不存在则创建
2. 编译源文件，生成目标文件（存储在 obj 目录中）
3. 编译资源文件（src/list.rc），生成 manifest.o
4. 链接所有目标文件和资源文件，生成可执行文件 danmaku.exe
5. 构建完成后，显示构建结果信息

### 3.3 依赖管理

项目使用 makefile 自动管理依赖关系，当源文件或头文件发生变化时，会自动重新编译相关文件。

## 4. 常见问题与解决方案

### 4.1 构建失败

#### 4.1.1 找不到编译器

**错误信息**：
```
make: gcc: Command not found
```

**解决方案**：
- 检查是否正确安装了 MSYS2 和 MingGW64
- 检查环境变量 `PATH` 是否包含 `C:\msys64\mingw64\bin`
- 尝试重新打开命令提示符，确保环境变量生效

#### 4.1.2 找不到头文件

**错误信息**：
```
fatal error: danmaku/dmkitem.hpp: No such file or directory
```

**解决方案**：
- 检查项目目录结构是否正确
- 确保头文件路径在 include 目录下
- 检查 makefile 中的包含路径设置

#### 4.1.3 链接错误

**错误信息**：
```
undefined reference to `GdiplusStartup'
```

**解决方案**：
- 检查是否正确链接了 GDI+ 库
- 确保 makefile 中的 LIBS 变量包含 `-lgdiplus`

### 4.2 运行时错误

#### 4.2.1 缺少 DLL 文件

**错误信息**：
```
无法启动此程序，因为计算机中缺少 libgcc_s_seh-1.dll。
```

**解决方案**：
- 将 MSYS2 中的 DLL 文件复制到可执行文件目录
- 或者将 MSYS2 的 bin 目录添加到系统环境变量 `PATH` 中

#### 4.2.2 权限错误

**错误信息**：
```
拒绝访问。
```

**解决方案**：
- 以管理员身份运行命令提示符或可执行文件
- 检查文件和目录的权限设置

## 5. 高级构建选项

### 5.1 自定义编译标志

可以通过命令行参数自定义编译标志：

```bash
make CXXFLAGS="-Wall -Wextra -g -std=c++17 -O2"
```

### 5.2 并行构建

使用 `-j` 选项可以启用并行构建，加快构建速度：

```bash
make -j4
```

其中 `4` 是并行任务数，可以根据 CPU 核心数调整。

### 5.3 交叉编译

项目支持交叉编译，可以在 64 位系统上构建 32 位版本：

```bash
make ARCH=32
```

## 6. 构建脚本

### 6.1 批处理脚本

可以创建批处理脚本简化构建过程，例如：

**build_debug.bat**：
```batch
@echo off
make
pause
```

**build_release.bat**：
```batch
@echo off
make RELEASE=1
pause
```

### 6.2 PowerShell 脚本

**build_debug.ps1**：
```powershell
make
Read-Host "Press Enter to continue..."
```

**build_release.ps1**：
```powershell
make RELEASE=1
Read-Host "Press Enter to continue..."
```

## 7. 持续集成

如果需要设置持续集成，可以参考以下配置：

### 7.1 GitHub Actions

在项目根目录下创建 `.github/workflows/build.yml` 文件：

```yaml
name: Build

on:
  push:
    branches: [ main, dev ]
  pull_request:
    branches: [ main, dev ]

jobs:
  build:
    runs-on: windows-latest

    steps:
    - uses: actions/checkout@v2

    - name: Set up MSYS2
      uses: msys2/setup-msys2@v2
      with:
        msystem: MINGW64
        update: true
        install: >-
          mingw-w64-x86_64-gcc
          mingw-w64-x86_64-make

    - name: Build DEBUG
      shell: msys2 {0}
      run: make

    - name: Build RELEASE
      shell: msys2 {0}
      run: make RELEASE=1

    - name: Build 32-bit DEBUG
      shell: msys2 {0}
      run: make ARCH=32

    - name: Build 32-bit RELEASE
      shell: msys2 {0}
      run: make ARCH=32 RELEASE=1
```

## 8. 总结

本指南详细说明了 DesktopDanmaku 项目的构建过程，包括环境搭建、构建命令和常见问题的解决方案。通过遵循本指南，您应该能够成功构建项目并解决构建过程中遇到的问题。

如果您在构建过程中遇到其他问题，请参考项目的 GitHub Issues 页面或联系项目维护者。

---

希望本指南对您有所帮助！