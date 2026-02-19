#ifndef DANMAKU_MAIN_WINDOW_HPP
#define DANMAKU_MAIN_WINDOW_HPP

#include <windows.h>
#include <string>
#include "overlay.hpp"

namespace danmaku
{

    // 主窗口类
    class MainWindow : public BaseWindow
    {
    private:
        // dm: danmaku main
        WNDCLASSEX dm_wc{};
        OverlayWindow overlay{};
    public:
        PCWSTR className() const override { return dm_wc.lpszClassName; }
        // 处理窗口消息
        LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        // 构造函数和析构函数
        MainWindow() = default;
        // 使用默认析构函数
        ~MainWindow() = default;
        // 创建和显示函数
        MainWindow &create();
        // 重载 create 函数以接受窗口标题、宽度和高度参数
        MainWindow &create(std::wstring title, int width, int height);
        // 显示窗口
        MainWindow &show();
        // 获取overlayWindow实例的引用
        OverlayWindow &getOverlay() { return overlay; }
    };
}

#endif // DANMAKU_MAIN_WINDOW_HPP