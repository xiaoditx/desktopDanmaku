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

// 全局元素对象，避免在函数结束后被销毁
std::vector<danmaku::element> g_elements;
// 默认字体对象，微软雅黑字体，大小为24
danmaku::font defaultFont(L"微软雅黑", 24);
danmaku::labelExtraInfo lei{
    RGB(0, 0, 0),      // 黑色文本
    RGB(255, 255, 255) // 白色背景
};
void init_creatElement(danmaku::baseWindow &mainWND)
{
    g_elements.reserve(3);
    /* 标签 */
    // 软件名
    g_elements.emplace_back(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{0, 0, 500, 25},
        L"桌面弹幕 - Desktop Danmaku",
        defaultFont,
        nullptr);
    // 输入提示
    g_elements.emplace_back(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{5, 30, 100, 25},
        L"内容：",
        defaultFont,
        &lei);
    /* 按钮 */
    g_elements.emplace_back(
        mainWND.getHandle(),
        danmaku::elementType::button,
        danmaku::rect{5, 60, 100, 25},
        L"发送弹幕",
        defaultFont,
        nullptr);
    // 创建元素
    danmaku::createElements(g_elements[0], g_elements[1], g_elements[2]);
}