#include "pch.hpp"
#include "windows/main.hpp"

namespace danmaku
{

    // 主窗口的创建函数（无参数版本）
    MainWindow &MainWindow::create()
    {
        // 调用带参数的 create 函数，使用默认标题和大小
        return create(L"桌面弹幕", 800, 600);
    }

    // 主窗口的创建函数（带参数版本）
    MainWindow &MainWindow::create(std::wstring title, int width, int height)
    {
        // 注册窗口类
        dm_wc.cbSize = sizeof(WNDCLASSEX);
        dm_wc.style = CS_HREDRAW | CS_VREDRAW;
        dm_wc.lpfnWndProc = BaseWindow::wndProc;
        dm_wc.cbClsExtra = 0;
        dm_wc.cbWndExtra = 0;
        dm_wc.hInstance = GetModuleHandle(nullptr);
        dm_wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        dm_wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        dm_wc.lpszMenuName = nullptr;
        dm_wc.lpszClassName = L"Danmaku.WndCls.Main";
        // dm_wc.hIcon = nullptr;//
        // dm_wc.hIconSm = nullptr;//

        if (!RegisterClassExW(&dm_wc))
        {
            debug::logOutput(L"[错误] 主窗口类注册错误\n");
            debug::logWinError(GetLastError());
            MessageBoxW(nullptr, L"（0001）主窗口：窗口类注册失败", L"出错了欸", MB_ICONERROR | MB_OK);
            return *this;
        }

        // 创建窗口
        hwnd = CreateWindowExW(
            0, dm_wc.lpszClassName, title.c_str(),
            // 禁止调整窗口大小和最大化
            WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            // 使用默认位置，指定宽高
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            // 父窗口和菜单都设为 nullptr，使用当前实例句柄，传递 this 指针以供消息处理使用
            nullptr, nullptr, GetModuleHandleW(nullptr), this);

        // 检查窗口创建是否成功
        if (!hwnd)
        {
            debug::logOutput(L"[错误] 主窗口创建失败\n");
            debug::logWinError(GetLastError());
            MessageBoxW(nullptr, L"（0002）主窗口：创建窗口失败", L"出错了欸", MB_ICONERROR | MB_OK);
        }

        overlay.create().show();

        return *this;
    }

    // 主窗口的显示函数
    MainWindow &MainWindow::show()
    {
        ShowWindow(hwnd, SW_SHOW);
        // UpdateWindow(hwnd);
        return *this;
    }

    inline HRESULT transferToElement(UINT_PTR id, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        try
        {
            return searchID(id).procMessage(uMsg, wParam, lParam);
        }
        catch (const std::exception &e)
        {
            return S_FALSE;
        }
    }

    // 主窗口的消息处理函数
    LRESULT MainWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CTLCOLORSTATIC:
        {
            HWND hWndCtrl = (HWND)lParam;
            UINT_PTR nCtrlID = GetDlgCtrlID(hWndCtrl);
            return transferToElement(nCtrlID, uMsg, wParam, lParam);
        }
        case WM_COMMAND:
        {
            int wmEvent = HIWORD(wParam);
            debug::logOutput(L"WM_COMMAND事件，ID：", LOWORD(wParam), L"，事件类型：", wmEvent, L"\n");
            // 按钮被点击
            if (wmEvent == BN_CLICKED)
            {
                debug::logOutput(L"按钮点击事件，ID：", LOWORD(wParam), L"\n");
                int wmId = LOWORD(wParam);
                return transferToElement(wmId, uMsg, wParam, lParam);
            }
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
        }

        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
        }
    }
} // namespace danmaku