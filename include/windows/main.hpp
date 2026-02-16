#ifndef DANMAKU_MAIN_WINDOW_HPP
#define DANMAKU_MAIN_WINDOW_HPP

#include <windows.h>
#include <string>
#include "overlay.hpp"

namespace danmaku
{

    // 主窗口类
    class mainWindow : public baseWindow
    {
    private:
        // dm: danmaku main
        WNDCLASSEX dm_wc{};
        overlayWindow overlay{};
    public:
        PCWSTR ClassName() const override { return dm_wc.lpszClassName; }
        // 处理窗口消息
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        // 构造函数和析构函数
        mainWindow() = default;
        // 使用默认析构函数
        ~mainWindow() = default;
        // 创建和显示函数
        mainWindow &create();
        // 重载 create 函数以接受窗口标题、宽度和高度参数
        mainWindow &create(std::wstring title, int width, int height);
        // 显示窗口
        mainWindow &show();
    };
}

#endif // DANMAKU_MAIN_WINDOW_HPP