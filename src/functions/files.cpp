#include "functions/files.hpp"
#include <filesystem>
#include <windows.h>

// 各种文件路径处理函数
bool GetExePath(wchar_t *outBuffer, size_t bufferSize)
{
    if (!outBuffer || bufferSize == 0)
    {
        return false;
    }

    DWORD result = GetModuleFileNameW(nullptr, outBuffer, static_cast<DWORD>(bufferSize));

    // 检查错误
    if (result == 0 || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        outBuffer[0] = L'\0';
        return false;
    }

    return true;
}

bool GetExeDirectory(wchar_t *outBuffer, size_t bufferSize)
{
    if (!outBuffer || bufferSize == 0)
    {
        return false;
    }

    // 先获取exe完整路径
    if (!GetExePath(outBuffer, bufferSize))
    {
        return false;
    }

    // 去掉文件名，只保留目录
    wchar_t *lastSlash = nullptr;
    wchar_t *current = outBuffer;
    while (*current)
    {
        if (*current == L'\\' || *current == L'/')
        {
            lastSlash = current;
        }
        ++current;
    }

    if (lastSlash)
    {
        *lastSlash = L'\0';
    }
    else
    {
        // 没有找到目录分隔符，返回当前目录
        DWORD result = GetCurrentDirectoryW(static_cast<DWORD>(bufferSize), outBuffer);
        return result > 0 && result <= bufferSize;
    }

    return true;
}

bool GetExeRelativePath(const wchar_t *rawPath, wchar_t *outBuffer, size_t bufferSize)
{
    if (!rawPath || !outBuffer || bufferSize == 0)
    {
        return false;
    }

    // 获取exe目录
    wchar_t exeDir[MAX_PATH];
    if (!GetExeDirectory(exeDir, MAX_PATH))
    {
        return false;
    }

    // 检查是否是一下情况的一种：UNC路径、根相对路径、绝对路径
    if (rawPath[0] == L'\\' || rawPath[0] == L'/' ||
        (rawPath[1] == L':' && (rawPath[2] == L'\\' || rawPath[2] == L'/')))
    {
        // 已经是绝对路径，直接复制
        if (wcslen(rawPath) >= bufferSize)
        {
            return false;
        }
        wcscpy_s(outBuffer, bufferSize, rawPath);
        return true;
    }

    // 组合路径
    std::filesystem::path fullPath = std::filesystem::path(exeDir) / std::filesystem::path(rawPath);

    // 规范化路径（处理..和.）
    fullPath = std::filesystem::weakly_canonical(fullPath);

    // 复制到输出缓冲区
    std::wstring fullPathStr = fullPath.wstring();
    if (fullPathStr.length() >= bufferSize)
    {
        return false;
    }

    wcscpy_s(outBuffer, bufferSize, fullPathStr.c_str());
    return true;
}

bool FileExists(const wchar_t *rawPath)
{
    wchar_t fullPath[MAX_PATH];

    // 获取基于exe目录的完整路径
    if (!GetExeRelativePath(rawPath, fullPath, MAX_PATH))
    {
        return false;
    }
    // 检查文件是否存在
    return std::filesystem::exists(fullPath);
}
