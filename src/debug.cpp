#include "pch.hpp"
#include "debug.hpp"
#include "functions/files.hpp"

namespace debug
{
    /**
     * @brief 获取完整的调试文件路径
     * @return 调试文件路径
     */
    wchar_t *getFullDebugFilePath()
    {
        static std::unique_ptr<wchar_t[]> fullPath = []()
        {
            auto buffer = std::make_unique<wchar_t[]>(MAX_PATH);
            GetExeRelativePath(L"./log.txt", buffer.get(), MAX_PATH);
            return buffer;
        }();
        return fullPath.get(); // 返回原始指针供外部使用
    }

    /**
     * @brief 记录Windows错误
     * @param errorCode 错误代码
     */
    void logWinError(DWORD errorCode)
    {
        LPWSTR messageBuffer = nullptr;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&messageBuffer, 0, nullptr);
        logOutput(L"    - 错误代码：", std::to_wstring(errorCode).c_str(), L"\n    - 错误信息：", messageBuffer, L"\n");
        LocalFree(messageBuffer);
    }

    /**
     * @brief 记录程序启动时间
     * 
     * @note 本函数将时间输出到日志
     */
    void logProgramStartTime()
    {
        // YYYY-MM-DDTHH:MM:SS
        SYSTEMTIME st;
        GetLocalTime(&st);
        debug::logOutput(
            L"[程序启动] ",
            std::to_wstring(st.wYear).c_str(),
            L"-", std::to_wstring(st.wMonth).c_str(),
            L"-", std::to_wstring(st.wDay).c_str(),
            L" ", std::to_wstring(st.wHour).c_str(),
            L":", std::to_wstring(st.wMinute).c_str(),
            L":", std::to_wstring(st.wSecond).c_str(),
            L"\n");
    }
}