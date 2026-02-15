#define _WIN32_IE 0x0600
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <commctrl.h>
#include "main.hpp"
#include <optional>

void init_creatElement(danmaku::baseWindow &mainWND);
int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
    // 初始化通用控件库
    INITCOMMONCONTROLSEX iccex;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccex.dwICC = ICC_STANDARD_CLASSES; // 包含按钮在内的标准控件
    // 如果你还需要更高级的控件（如列表视图、树形视图），可以用 ICC_WIN95_CLASSES
    // iccex.dwICC = ICC_WIN95_CLASSES;
    if (!InitCommonControlsEx(&iccex))
    {
        // 处理错误（通常不会失败）
        MessageBox(NULL, L"初始化通用控件失败", L"错误", MB_OK);
        return 1;
    }

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

// 默认字体对象，微软雅黑字体，大小为24
danmaku::font defaultFont(L"微软雅黑", 24);
danmaku::labelExtraInfo lei{
    RGB(0, 0, 0),      // 黑色文本
    RGB(255, 255, 255) // 白色背景
};

// 全局 optional 变量
std::optional<danmaku::element> g_elemLabelAppName;
std::optional<danmaku::element> g_elemLabelPrompt;
std::optional<danmaku::element> g_elemEditContent;
std::optional<danmaku::element> g_elemButton;

void init_creatElement(danmaku::baseWindow &mainWND)
{
    // 软件名标签
    g_elemLabelAppName.emplace(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{0, 0, 500, 25},
        L"桌面弹幕 - Desktop Danmaku",
        defaultFont,
        &lei);

    // 输入提示标签
    g_elemLabelPrompt.emplace(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{5, 40, 100, 28},
        L"内容：",
        defaultFont,
        &lei);

    // 输入框
    g_elemEditContent.emplace(
        mainWND.getHandle(),
        danmaku::elementType::edit,
        // 输入框高度必须>=28，这是适应字体大小
        // 字体设置为24，但实际会更大些，编辑框高度小于28会使上边框变白
        danmaku::rect{60, 40, 300, 28},
        L"",
        defaultFont,
        nullptr);

    // 发送按钮
    g_elemButton.emplace(
        mainWND.getHandle(),
        danmaku::elementType::button,
        danmaku::rect{370, 40, 100, 28},
        L"发送弹幕",
        defaultFont,
        nullptr);

    // 创建元素（通过 .value() 获取引用）
    danmaku::createElements(
        g_elemLabelAppName.value(),
        g_elemLabelPrompt.value(),
        g_elemEditContent.value(),
        g_elemButton.value());
}