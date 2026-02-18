#ifndef DANMAKU_BASE_WINDOW_HPP
#define DANMAKU_BASE_WINDOW_HPP

#include <windows.h>

namespace danmaku
{
    /**
     * @brief 基础窗口类，提供窗口创建、显示和消息处理的基本框架
     */
    class baseWindow
    {
    public:
        baseWindow() = default;
        virtual ~baseWindow() = default;
        // 创建、显示
        virtual baseWindow &create() = 0;
        virtual baseWindow &show() = 0;

    protected:
        // 返回窗口类的名称
        virtual PCWSTR className() const = 0;
        // 处理窗口消息
        virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
        // 窗口句柄
        HWND hwnd = nullptr;

    public:
        // 模板化的窗口过程函数，用于将窗口消息分发到对应的窗口类实例
        static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            // 触发消息处理的窗口类的指针（初始为nullptr）
            baseWindow *window = nullptr;
            // 消息：窗口正在创建
            if (msg == WM_NCCREATE)
            {
                // 获取创建窗口时传入的类指针并存储在窗口的用户数据中
                CREATESTRUCT *creatInfo = (CREATESTRUCT *)lParam;
                window = (baseWindow *)creatInfo->lpCreateParams;
                SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)window);
                // 将窗口句柄存储在窗口类实例中以供后续消息处理使用
                window->hwnd = hwnd;
            }
            else
            {
                // 对于其他消息，尝试从窗口的用户数据中获取窗口类的指针
                window = (baseWindow *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
            }
            if (window)
            {
                // 如果成功获取到窗口类的指针，则调用该类的消息处理函数来处理消息
                return window->handleMessage(msg, wParam, lParam);
            }
            else
            {
                // 如果未能获取到窗口类的指针，则调用默认的窗口过程来处理消息
                return DefWindowProcW(hwnd, msg, wParam, lParam);
            }
        }
        HWND getHandle() const { return hwnd; }
    };
}

#endif // DANMAKU_BASE_WINDOW_HPP