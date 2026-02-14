#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <fstream>
#include <codecvt>
#include <locale>

namespace debug
{
    wchar_t *getFullDebugFilePath();
    /**
     * @brief 将调试信息输出到日志文件
     *
     * @param args 可变参数列表，支持多个参数传递
     *
     * @note 参数类型应支持宽字符输出流（std::wofstream）
     * @note 日志文件路径由全局变量`fullDebugFilePath`指定
     * @note 日志文件以UTF-8编码格式保存
     * @note 如果日志文件无法打开，函数将静默失败以避免影响主程序运行
     * @note 每次调用函数时，日志内容将被追加到文件末尾
     */
    template <typename... Args>
    void logOutput(Args... args)
    {
        // 使用追加模式打开文件，确保每次调用不会覆盖之前的日志
        std::wofstream logFile(getFullDebugFilePath(), std::ios::app);
        if (!logFile.is_open())
        {
            // 如果文件无法打开，可以在这里添加错误处理
            // 当前实现选择静默失败，避免影响主程序运行
            return;
        }
        // 设置locale以支持宽字符写入
        logFile.imbue(std::locale(logFile.getloc(), new std::codecvt_utf8<wchar_t>));
        // 写入消息
        ((logFile << args), ...);
        // 文件会在析构时自动关闭
    }

    /**
     * @brief 将Windows API错误代码转换为可读的错误信息并输出到日志文件
     *
     * @param errorCode Windows API返回的错误代码
     */
    void logWinError(DWORD errorCode);

    /**
     * @brief 记录程序启动时间到日志文件
     *
     * @note 时间格式为不标准的ISO 8601格式（YYYY-MM-DD HH:MM:SS）
     * @note 使用系统当前时间，时区为本地时区
     */
    void logProgramStartTime();
}

#endif // DEBUG_HPP