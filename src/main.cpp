#include <windows.h>
#include "main.hpp"

// 默认字体对象，微软雅黑字体，大小为24
danmaku::font defaultFont(L"微软雅黑", 24);
// 主窗口
danmaku::mainWindow mainWindowObj;
/* 标签 */
// 软件名
danmaku::elementInfo appNameLabel{
    danmaku::elements::label,
    0, 0, 500, 25, L"桌面弹幕 - Desktop Danmaku",
    defaultFont, nullptr};
/* 按钮 */
danmaku::elementInfo buttonInfo{
    danmaku::elements::label,
    0, 0, 500, 25, L"桌面弹幕 - Desktop Danmaku",
    defaultFont, nullptr};

int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
    mainWindowObj.create(L"桌面弹幕", 500, 300)
        .createElements(appNameLabel)
        .show();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}