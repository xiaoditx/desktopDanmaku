#include <windows.h>
#include "main.hpp"

void init_creatElement(danmaku::baseWindow &mainWND);
int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
    // 主窗口
    danmaku::mainWindow mainWindowObj;
    mainWindowObj.create(L"桌面弹幕", 500, 300).show();

    // 初始化元素
    init_creatElement(mainWindowObj);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

void init_creatElement(danmaku::baseWindow &mainWND)
{
    // 默认字体对象，微软雅黑字体，大小为24
    danmaku::font defaultFont(L"微软雅黑", 24);
    /* 标签 */
    // 软件名
    danmaku::element appNameLabel{
        mainWND.getHandle(),
        danmaku::elementType::label,
        {0, 0, 500, 25},
        L"桌面弹幕 - Desktop Danmaku",
        defaultFont,
        nullptr};
    // 输入提示
    danmaku::element inputHintLabel{
        mainWND.getHandle(),
        danmaku::elementType::label,
        {5, 30, 100, 25},
        L"内容：",
        defaultFont,
        nullptr};
    /* 按钮 */
    danmaku::labelExtraInfo lei{
        RGB(255, 255, 255), // 白色文本
        RGB(0, 120, 215)    // Windows 10 默认蓝色背景
    };
    danmaku::element buttonInfo{
        mainWND.getHandle(),
        danmaku::elementType::button,
        {5, 60, 100, 25},
        L"发送弹幕",
        defaultFont,
        &lei};
    createElements(appNameLabel, inputHintLabel, buttonInfo);
}