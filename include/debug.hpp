#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "pch.hpp"

namespace debug
{
    wchar_t *getFullDebugFilePath();

    std::string WideToUtf8(const std::wstring& wstr);

    template <typename... Args>
    void logOutput(Args... args)
    {
        std::ofstream logFile(getFullDebugFilePath(), std::ios::app);
        if (!logFile.is_open())
        {
            return;
        }
        std::wostringstream wss;
        ((wss << args), ...);
        logFile << WideToUtf8(wss.str());
    }

    void logWinError(DWORD errorCode);

    void logProgramStartTime();
}

#endif // DEBUG_HPP