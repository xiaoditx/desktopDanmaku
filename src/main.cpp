#define _WIN32_IE 0x0600
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <commctrl.h>
#include <optional>
#include <gdiplus.h>
#include "main.hpp"

// 向前声明
void init_creatElement(danmaku::baseWindow &mainWND);
void buttonClickHandler();

// 全局 GDI+ token
ULONG_PTR g_gpToken{};
// 全局 overlayWindow 指针，用于在按钮点击时添加弹幕
danmaku::overlayWindow *g_overlayWindow = nullptr;
// 全局 optional 变量
std::optional<danmaku::element> g_elemLabelAppName;
std::optional<danmaku::element> g_elemLabelPrompt;
std::optional<danmaku::element> g_elemEditContent;
std::optional<danmaku::element> g_elemButton;

int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
    // 初始化通用控件库
    INITCOMMONCONTROLSEX iccex;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccex.dwICC = ICC_STANDARD_CLASSES; // 包含按钮在内的标准控件
    // 如果还需要更高级的控件（如列表视图、树形视图），可以用 ICC_WIN95_CLASSES
    // iccex.dwICC = ICC_WIN95_CLASSES;
    if (!InitCommonControlsEx(&iccex))
    {
        // 处理错误（通常不会失败）
        MessageBoxW(nullptr, L"初始化通用控件失败", L"错误", MB_OK);
        return 1;
    }

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    if (Gdiplus::GdiplusStartup(
            &g_gpToken,
            &gdiplusStartupInput,
            nullptr) != Gdiplus::Ok)
    {
        MessageBoxW(nullptr, L"初始化 GDI+ 失败", L"错误", MB_OK);
        return 1;
    }

    // 主窗口
    MSG msg;
    {
        danmaku::mainWindow mainWindowObj;
        mainWindowObj.create(L"桌面弹幕", 500, 300).show();

        // 获取overlayWindow实例的指针
        g_overlayWindow = &mainWindowObj.getOverlay();

        // 初始化元素
        init_creatElement(mainWindowObj);

        while (GetMessageW(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    } // GDI+关闭前析构

    Gdiplus::GdiplusShutdown(g_gpToken);

    return (int)msg.wParam;
}

// 默认字体对象，微软雅黑字体，大小为24
danmaku::font defaultFont(L"微软雅黑", 24);
danmaku::labelExtraInfo lei{
    RGB(0, 0, 0),      // 黑色文本
    RGB(255, 255, 255) // 白色背景
};
danmaku::buttonExtraInfo bei{
    buttonClickHandler,
    nullptr, // 暂无
    nullptr  // 暂无
};

void init_creatElement(danmaku::baseWindow &mainWND)
{
    // 软件名标签
    g_elemLabelAppName.emplace(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{0, 5, 500, 25},
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
        &bei);

    // 创建元素（通过 .value() 获取引用）
    danmaku::createElements(
        g_elemLabelAppName.value(),
        g_elemLabelPrompt.value(),
        g_elemEditContent.value(),
        g_elemButton.value());
}

void buttonClickHandler()
{
    // 获取输入框内容
    wchar_t szBuffer[256]{};
    GetDlgItemTextW(g_elemEditContent->getParentHwnd(), g_elemEditContent->getID(), szBuffer, 256);
    std::wstring content = szBuffer;
    if (content.empty())
    {
        MessageBoxW(nullptr, L"请输入弹幕内容！", L"提示", MB_OK);
        return;
    }
    if(countVisibleCharacters(content) > 80)
    {
        MessageBoxW(nullptr, L"弹幕内容不能超过80个字符！", L"提示", MB_OK);
        return;
    }

    // 弹幕发送逻辑
    if (g_overlayWindow)
    {
        // 添加新弹幕
        g_overlayWindow->addDanmaku(content, 40, 0xff'ffffff, 0xff'000000);
    }

    // 发送后清空输入框
    //SetDlgItemTextW(g_elemEditContent->getParentHwnd(), g_elemEditContent->getID(), L"");
}