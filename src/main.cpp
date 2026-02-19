#include "pch.hpp"
#include "main.hpp"

// 向前声明
void init_creatElement(danmaku::BaseWindow &mainWND);
void buttonClickHandler();

// 全局 GDI+ token
ULONG_PTR g_gpToken{};
// 全局 OverlayWindow 指针，用于在按钮点击时添加弹幕
danmaku::MainWindow *g_mainWindow = nullptr;
danmaku::OverlayWindow *g_overlayWindow = nullptr;
// 全局 optional 变量
std::optional<danmaku::Element> g_elemLabelAppName;
std::optional<danmaku::Element> g_elemLabelPrompt;
std::optional<danmaku::Element> g_elemEditContent;
std::optional<danmaku::Element> g_elemButton;
// ↓ 后期将会禁用
std::optional<danmaku::Element> g_elemLabelColor1;
std::optional<danmaku::Element> g_elemEditColor1;
std::optional<danmaku::Element> g_elemLabelColor2;
std::optional<danmaku::Element> g_elemEditColor2;

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

    danmaku::DanmakuBitmapCache::startup();

    assert(!g_mainWindow);
    g_mainWindow = new danmaku::MainWindow{};
    g_mainWindow->create(L"桌面弹幕", 500, 300).show();
    // 获取overlayWindow实例的指针
    g_overlayWindow = &g_mainWindow->getOverlay();

    // 初始化元素
    init_creatElement(*g_mainWindow);

    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!IsDialogMessageW(g_mainWindow->getHandle(), &msg))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    delete g_mainWindow; // GDI+关闭前析构

    danmaku::DanmakuBitmapCache::shutdown();
    Gdiplus::GdiplusShutdown(g_gpToken);

    return (int)msg.wParam;
}

// 默认字体对象，微软雅黑字体，大小为24
danmaku::Font defaultFont(L"微软雅黑", 24);
danmaku::LabelExtraInfo lei{
    RGB(0, 0, 0),      // 黑色文本
    RGB(255, 255, 255) // 白色背景
};
danmaku::ButtonExtraInfo bei{
    buttonClickHandler,
    nullptr, // 暂无
    nullptr  // 暂无
};

void init_creatElement(danmaku::BaseWindow &mainWND)
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

    // 选择文本颜色(提示标签)
    g_elemLabelColor1.emplace(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{5, 80, 100, 28},
        L"文本颜色：",
        defaultFont,
        &lei);

    // 选择文本颜色(输入框)
    g_elemEditColor1.emplace(
        mainWND.getHandle(),
        danmaku::elementType::edit,
        danmaku::rect{90, 80, 380, 28},
        L"",
        defaultFont,
        nullptr);

    // 选择边框颜色(提示标签)
    g_elemLabelColor2.emplace(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{5, 120, 100, 28},
        L"边框颜色：",
        defaultFont,
        &lei);

    // 选择边框颜色(输入框)
    g_elemEditColor2.emplace(
        mainWND.getHandle(),
        danmaku::elementType::edit,
        danmaku::rect{90, 120, 380, 28},
        L"",
        defaultFont,
        nullptr);

    // 创建元素（通过 .value() 获取引用）
    danmaku::createElements(
        g_elemLabelAppName.value(),
        g_elemLabelPrompt.value(),
        g_elemEditContent.value(),
        g_elemButton.value(),
        g_elemLabelColor1.value(),
        g_elemEditColor1.value(),
        g_elemLabelColor2.value(),
        g_elemEditColor2.value());
}

Gdiplus::ARGB HexStringToARGB(const std::wstring &hexStr)
{
    try
    {
        unsigned long value = std::stoul(hexStr, nullptr, 16);
        return static_cast<Gdiplus::ARGB>(value);
    }
    catch (const std::invalid_argument &)
    {
        // 字符串不包含有效数字
        return 0xFF000000; // 默认返回黑色不透明
    }
    catch (const std::out_of_range &)
    {
        // 数值超出 unsigned long 范围
        return 0xFFFFFFFF; // 返回白色不透明
    }
}

void buttonClickHandler()
{
    // 获取输入框内容
    wchar_t szBuffer[256]{};
    GetDlgItemTextW(g_elemEditContent->getParentHwnd(), g_elemEditContent->getID(), szBuffer, _countof(szBuffer));
    std::wstring content = szBuffer;
    if (content.empty())
    {
        MessageBoxW(g_mainWindow->getHandle(), L"请输入弹幕内容！", L"提示", MB_OK);
        return;
    }
    if (countVisibleCharacters(content) > 80)
    {
        MessageBoxW(g_mainWindow->getHandle(), L"弹幕内容不能超过80个字符！", L"提示", MB_OK);
        return;
    }

    GetDlgItemTextW(g_elemEditColor1->getParentHwnd(), g_elemEditColor1->getID(), szBuffer, _countof(szBuffer));
    std::wstring color1 = szBuffer;
    if (color1.empty())
    {
        // 默认颜色（白色不透明）
        color1 = L"0xffffffff";
    }
    GetDlgItemTextW(g_elemEditColor2->getParentHwnd(), g_elemEditColor2->getID(), szBuffer, _countof(szBuffer));
    std::wstring color2 = szBuffer;
    if (color2.empty())
    {
        // 默认颜色（黑色不透明）
        color2 = L"0xff000000";
    }

    // 弹幕发送逻辑
    if (g_overlayWindow)
    {
        // 添加新弹幕
        g_overlayWindow->addDanmaku(content, 40, HexStringToARGB(color1), HexStringToARGB(color2));
    }

    // 发送后清空输入框
    // SetDlgItemTextW(g_elemEditContent->getParentHwnd(), g_elemEditContent->getID(), L"");
}