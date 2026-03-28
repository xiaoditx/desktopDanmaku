#include "main.hpp"

// 向前声明
void init_creatElement(danmaku::BaseWindow &mainWND);
void buttonClickHandler();
void jsonButtonClickHandler();

// 全局 GDI+ token
ULONG_PTR g_gpToken{};
// 全局 OverlayWindow 指针
danmaku::MainWindow *g_mainWindow = nullptr;
danmaku::OverlayWindow *g_overlayWindow = nullptr;
// 主窗口组件
std::optional<danmaku::Element> g_elemLabelAppName;
std::optional<danmaku::Element> g_elemLabelPrompt;
std::optional<danmaku::Element> g_elemEditContent;
std::optional<danmaku::Element> g_elemButton;
std::optional<danmaku::Element> g_elemJsonLabel;
std::optional<danmaku::Element> g_elemJsonButton;
std::optional<danmaku::Element> g_elemJsonEdit;
// std::optional<danmaku::Element> g_elemChoiceFile;
// ↓ 这些组件后期将会禁用
std::optional<danmaku::Element> g_elemLabelColor1;
std::optional<danmaku::Element> g_elemEditColor1;
std::optional<danmaku::Element> g_elemLabelColor2;
std::optional<danmaku::Element> g_elemEditColor2;

// todo: 写一个提供日志输出、弹窗警告等功能的错误处理函数
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

    // 初始化GDI
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    if (Gdiplus::GdiplusStartup(
            &g_gpToken,
            &gdiplusStartupInput,
            nullptr) != Gdiplus::Ok)
    {
        // 错误处理
        MessageBoxW(nullptr, L"初始化 GDI+ 失败", L"错误", MB_OK);
        return 1;
    }

    // 初始化弹幕位图缓存
    danmaku::DanmakuBitmapCache::startup();

    assert(!g_mainWindow);
    g_mainWindow = new danmaku::MainWindow{};
    g_mainWindow->create(L"桌面弹幕", 500, 300).show();
    // 获取overlayWindow实例的指针
    g_overlayWindow = &g_mainWindow->getOverlay();

    // 初始化元素
    init_creatElement(*g_mainWindow);

    // danmaku::ReadDanmakuArrayFromJsonFile(L"./test.json");

    // 消息循环
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!IsDialogMessageW(g_mainWindow->getHandle(), &msg))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    // 各种资源的销毁
    if (g_mainWindow)
        delete g_mainWindow; // GDI+关闭前析构
    g_mainWindow = nullptr;

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
danmaku::ButtonExtraInfo bei_json{
    jsonButtonClickHandler,
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

    // 从JSON文件加载弹幕的标签
    g_elemJsonLabel.emplace(
        mainWND.getHandle(),
        danmaku::elementType::label,
        danmaku::rect{5, 160, 120, 28},
        L"JSON文件",
        defaultFont,
        &lei);

    // 从JSON文件加载弹幕的按钮
    g_elemJsonButton.emplace(
        mainWND.getHandle(),
        danmaku::elementType::button,
        danmaku::rect{90, 160, 100, 28},
        L"读取文件",
        defaultFont,
        &bei_json);

    // 从JSON文件加载弹幕的输入框
    g_elemJsonEdit.emplace(
        mainWND.getHandle(),
        danmaku::elementType::edit,
        danmaku::rect{200, 160, 270, 28},
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
        g_elemEditColor2.value(),
        g_elemJsonLabel.value(),
        g_elemJsonButton.value(),
        g_elemJsonEdit.value()
        // ,g_elemChoiceFile.value(),
    );
}

// 按钮按下事件处理函数
void buttonClickHandler()
{
    // 获取输入框内容
    wchar_t buffer[256]{};
    UINT length;

    // 获取颜色输入框的内容并转换为ARGB颜色值
    length = GetDlgItemTextW(
        g_elemEditColor1->getParentHwnd(),
        g_elemEditColor1->getID(),
        buffer, _countof(buffer));
    // 如果用户输入了颜色值，则转换；
    // 否则使用默认颜色（调试模式下随机颜色，发布模式下白色不透明）
    Gdiplus::ARGB color1;
    if (length)
        color1 = hexStringToArgb({buffer, length});
    else
    {
#ifdef _DEBUG
        color1 = (random::getInt(0x50, 0xFF) << 24) | random::getInt(0, 0xFFFFFF);
#else
        // 默认颜色（白色不透明）
        color1 = 0xff'ffffff;
#endif
    }

    // 下面内容同上
    length = GetDlgItemTextW(
        g_elemEditColor2->getParentHwnd(),
        g_elemEditColor2->getID(),
        buffer, _countof(buffer));
    Gdiplus::ARGB color2;
    if (length)
        color2 = hexStringToArgb({buffer, length});
    else
    {
#ifdef _DEBUG
        color2 = (random::getInt(0x50, 0xFF) << 24) | random::getInt(0, 0xFFFFFF);
#else
        // 默认颜色（黑色不透明）
        color2 = 0xff'000000;
#endif
    }

    // 获取弹幕内容输入框的文本
    length = GetDlgItemTextW(
        g_elemEditContent->getParentHwnd(),
        g_elemEditContent->getID(),
        buffer, _countof(buffer));
    if (!length)
    {
        // 如果输入框为空，弹出提示并返回
        MessageBoxW(g_mainWindow->getHandle(), L"请输入弹幕内容！", L"提示", MB_OK);
        return;
    }
    if (countVisibleCharacters({buffer, length}) > 80)
    {
        // 如果输入的内容超过80个可见字符，弹出提示并返回
        MessageBoxW(g_mainWindow->getHandle(), L"弹幕内容不能超过80个字符！", L"提示", MB_OK);
        return;
    }

    // 弹幕发送逻辑
    if (g_overlayWindow)
    {
        // 添加新弹幕
        g_overlayWindow->addDanmaku({buffer, length}, 40, color1, color2);
    }

    // 发送后清空输入框
    // todo 写一个选择框控制是否清空
    // if(checkbox.choice == false)
    SetDlgItemTextW(g_elemEditContent->getParentHwnd(), g_elemEditContent->getID(), L"");
}

void addDanmakuThread();
void jsonButtonClickHandler()
{
    std::thread addThread(addDanmakuThread);
    addThread.detach();
}

void addDanmakuThread()
{
    // 获取输入框内容
    wchar_t buffer[MAX_PATH]{};
    UINT length;

    // 获取弹幕内容输入框的文本
    length = GetDlgItemTextW(
        g_elemJsonEdit->getParentHwnd(),
        g_elemJsonEdit->getID(),
        buffer, _countof(buffer));
    if (!length)
    {
        // 如果输入框为空，弹出提示并返回
        MessageBoxW(g_mainWindow->getHandle(), L"请输入弹幕内容！", L"提示", MB_OK);
        return;
    }

    auto danmakuArr = danmaku::ReadDanmakuArrayFromJsonFile({buffer, length});
    if (danmakuArr.empty())
    {
        // 如果解析失败或数组为空，弹出提示并返回
        MessageBoxW(g_mainWindow->getHandle(), L"未能从JSON文件加载弹幕！请检查文件路径和内容格式。", L"提示", MB_OK);
        return;
    }
    for (const auto &item : danmakuArr)
    {
        if (g_overlayWindow)
        {
            g_overlayWindow->addDanmaku(item.getText(), item.getEmSize(), item.getFillColor(), item.getBorderColor());
        }
        // todo 应用time参数，定时显示
        Sleep(100); // 模拟处理延迟
    }
}