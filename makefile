# 设置代码页为UTF-8以支持中文路径和输出
$(shell chcp 65001 >nul)

DEBUG ?= 1

# 编译架构选择，默认64位
ARCH ?= 64

# 使用cmd为shell并禁止所有隐式规则
SHELL         := cmd
.SUFFIXES:

ifeq ($(ARCH),32)
CXX      = i686-w64-mingw32-g++
WINDRES  = windres
WINDRES_FLAG = -F pe-i386 -o
else
CXX      = g++
WINDRES  = windres
WINDRES_FLAG = -F pe-x86-64 -o
endif

ifeq ($(DEBUG), 1)
    CONFIG    := debug
    EXTRA_FLAGS := -O0 -g -D_DEBUG
else
    CONFIG    := release
    EXTRA_FLAGS := -O3 -s -DNDEBUG
endif

# 基本变量 
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic $(EXTRA_FLAGS) -DUNICODE -D_UNICODE
LDFLAGS  = -mwindows -municode
LDLIBS   = -luser32 -lgdi32 -lcomctl32 -lgdiplus

# 目录变量
SRC_DIR   := src
INC_DIR   := include
BUILD_BASE:= build
BUILD_DIR := $(BUILD_BASE)/$(ARCH)/$(CONFIG)
OBJ_DIR   := $(BUILD_DIR)/obj
BIN       := $(BUILD_DIR)/danmaku.exe

# 源文件列表 
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/windows/*.cpp) $(wildcard $(SRC_DIR)/functions/*.cpp) $(wildcard $(SRC_DIR)/danmaku/*.cpp)
#$(wildcard $(SRC_DIR)/.../*.cpp)

# 自动推导对象 
CXX_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXX_SRCS))

# 默认目标（64位debug模式）
.PHONY: all clean help run release release64 release32 installer installer64 installer32
all: $(BIN)
	@echo $(CONFIG)版本构建完成: $@

debug:
	@$(MAKE) DEBUG=1 all

# 链接 
$(BIN): $(CXX_OBJS) $(OBJ_DIR)/manifest.o
	@echo 正在链接生成可执行文件 $@ ...
	@$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/manifest.o: src/list.rc
	@echo 正在编译资源文件 $< ...
	@windres -o $@ $<

# 编译对象文件 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo 正在将源码文件 $< 编译到 $@
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -g -I$(INC_DIR) -MMD -MP -c $< -o $@

# 自动依赖 
-include $(CXX_OBJS:.o=.d)

# 目录创建（对象文件目录和资源对象目录）
$(OBJ_DIR):
	@echo 正在创建对象文件目录 "$(OBJ_DIR)"
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"

# 文件清理
clean:
	@echo 正在清理build目录 ...
	@if exist "$(BUILD_BASE)" rmdir /S /Q "$(BUILD_BASE)"
	@echo 清理完成

# 帮助 
help:
	@type .\docs\makefileHelper.txt

# 运行编译结果（默认64位）
run: $(BIN)
	@echo [运行] 运行 $(BIN)
	@$(BIN)

# 构建发布版（64+32）
release: clean release64 release32
	@echo 所有发布版本构建完成

# 发布版64位构建
release64:
	@echo 正在构建64位发布版本 ...
	@$(MAKE) ARCH=64 DEBUG=0 all

# 发布版32位构建
release32:
	@echo 正在构建32位发布版本 ...
	@$(MAKE) ARCH=32 DEBUG=0 all

# 构建安装包（64+32）
installer: clean release64 installer64 release32 installer32
	@echo 所有安装程序构建完成

# 64位安装包构建
installer64: installer.iss release64
	@echo 正在构建64位安装程序 ...
	@start cmd /c chcp 936 ^&^& iscc /DMyAppArch=64 installer.iss
	@echo 64位安装程序编译完毕

# 32位安装包构建
installer32: installer.iss release32
	@echo 正在构建32位安装程序 ...
	@start cmd /c chcp 936 ^&^& iscc /DMyAppArch=32 installer.iss
	@echo 32位安装程序编译完毕