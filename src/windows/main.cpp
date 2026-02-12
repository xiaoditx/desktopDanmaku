#include "windows/main.hpp"

namespace danmaku
{

    // 主窗口的创建函数（无参数版本）
    mainWindow &mainWindow::create()
    {
        // 调用带参数的 create 函数，使用默认标题和大小
        return create(L"桌面弹幕", 800, 600);
    }

    // 主窗口的创建函数（带参数版本）
    mainWindow &mainWindow::create(std::wstring title, int width, int height)
    {
        // 注册窗口类
        // ZeroMemory(&dm_wc, sizeof(WNDCLASSEX));//
        dm_wc.cbSize = sizeof(WNDCLASSEX);
        dm_wc.style = CS_HREDRAW | CS_VREDRAW;
        dm_wc.lpfnWndProc = baseWindow::wndProc<mainWindow>;
        dm_wc.cbClsExtra = 0;
        dm_wc.cbWndExtra = 0;
        dm_wc.hInstance = GetModuleHandle(nullptr);
        // dm_wc.hIcon = nullptr;//
        // dm_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);//
        dm_wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        dm_wc.lpszMenuName = nullptr;
        dm_wc.lpszClassName = L"DanmakuMainWindowClass";
        // dm_wc.hIconSm = nullptr;//

        if (!RegisterClassEx(&dm_wc))
        {
            MessageBox(nullptr, L"（0001）主窗口：窗口类注册失败", L"出错了欸", MB_ICONERROR | MB_OK);
            return *this;
        }

        // 创建窗口
        hwnd = CreateWindowEx(
            0, dm_wc.lpszClassName, title.c_str(),
            // 禁止调整窗口大小和最大化
            WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
            // 使用默认位置，指定宽高
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            // 父窗口和菜单都设为 nullptr，使用当前实例句柄，传递 this 指针以供消息处理使用
            nullptr, nullptr, GetModuleHandle(nullptr), this);

        // 检查窗口创建是否成功
        if (!hwnd)
        {
            MessageBox(nullptr, L"（0002）主窗口：创建窗口失败", L"出错了欸", MB_ICONERROR | MB_OK);
        }
        return *this;
    }

    // 主窗口的显示函数
    mainWindow &mainWindow::show()
    {
        ShowWindow(hwnd, SW_SHOW);
        // UpdateWindow(hwnd);
        return *this;
    }

    // 主窗口的消息处理函数
    LRESULT mainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    mainWindow &mainWindow::createElements(elementInfo &elements)
    {
        return createElements(&elements, 1);
    }
    mainWindow &mainWindow::createElements(elementInfo *elements, int count)
    {
        for (int forCounter = 0; forCounter < count; ++forCounter)
        {
            elementInfo &elem = elements[forCounter];
            bool needFont = false;
            switch (elem.type)
            {
            case elements::button:
                needFont = true;
                elem.hwnd = CreateWindow(L"BUTTON", elem.text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                         elem.x, elem.y, elem.width, elem.height, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
                break;
            case elements::edit:
                needFont = true;
                elem.hwnd = CreateWindow(L"EDIT", elem.text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT,
                                         elem.x, elem.y, elem.width, elem.height, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
                break;
            case elements::label:
                needFont = true;
                elem.hwnd = CreateWindow(L"STATIC", elem.text.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT,
                                         elem.x, elem.y, elem.width, elem.height, hwnd, nullptr, GetModuleHandle(nullptr), nullptr);
                break;
            default:
                MessageBox(hwnd, L"（0003）主窗口：尝试创建未知的元素类型", L"出错了欸", MB_ICONERROR | MB_OK);
                break;
            }
            if (needFont && elem.elementFont)
            {
                SendMessage(elem.hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(elem.elementFont), TRUE);
            }
        }
        return *this;
    }

} // namespace danmaku