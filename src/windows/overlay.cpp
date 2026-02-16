#include "windows/overlay.hpp"

using namespace Gdiplus::DllExports;

namespace danmaku
{
    static HMONITOR getPrimaryMonitor()
    {
        HMONITOR monitor{};
        EnumDisplayMonitors(nullptr, nullptr, [](HMONITOR hMonitor, HDC, RECT *, LPARAM lParam) -> BOOL
                            {
            MONITORINFO mi;
            mi.cbSize = sizeof(mi);
            GetMonitorInfoW(hMonitor, &mi);
            if (mi.dwFlags & MONITORINFOF_PRIMARY)
            {
                *(HMONITOR*)lParam = hMonitor;
                return FALSE;
            }
            else
                return TRUE; }, (LPARAM)&monitor);
        return monitor;
    }

    BOOL overlayWindow::layoutFullscreen()
    {
        const auto mon = getPrimaryMonitor();
        if (!mon)
            return FALSE;
        MONITORINFO mi;
        mi.cbSize = sizeof(mi);
        if (GetMonitorInfoW(mon, &mi))
        {
            return SetWindowPos(hwnd, nullptr,
                                mi.rcWork.left, mi.rcWork.top,
                                mi.rcWork.right - mi.rcWork.left,
                                mi.rcWork.bottom - mi.rcWork.top,
                                SWP_NOZORDER | SWP_NOACTIVATE);
        }
        return FALSE;
    }

    void overlayWindow::recreateMemoryDC()
    {
        const auto dc = GetDC(hwnd);
        if (!cdc_)
        {
            cdc_ = CreateCompatibleDC(dc);
            SetBkMode(cdc_, TRANSPARENT);
            SetTextColor(cdc_, 0xffcc66);
        }
        else
        {
            SelectObject(cdc_, oldObject_);
        }
        DeleteObject(bitmap_);
        bitmap_ = CreateCompatibleBitmap(dc, width_, height_);
        oldObject_ = SelectObject(cdc_, bitmap_);

        if (graphics_)
            GdipDeleteGraphics(graphics_);
        GdipCreateFromHDC(cdc_, &graphics_);

        paint();
    }

    void overlayWindow::paint()
    {
        const RECT rc{0, 0, width_, height_};
        FillRect(cdc_, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

        for (int i = 0; i < 1; ++i)
        {
            int x = 200, y = 100;
            for (auto &item : danmaku_)
            {
                item.draw(graphics_, x, y);
                x += 250;
                y += 90;
            }
        }

        constexpr BLENDFUNCTION BlendFuncAlpha{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
        constexpr POINT SourcePoint{};
        const SIZE size{width_, height_};
        UPDATELAYEREDWINDOWINFO ulwi{};
        ulwi.cbSize = sizeof(ulwi);
        ulwi.psize = &size;
        ulwi.hdcSrc = cdc_;
        ulwi.pptSrc = &SourcePoint;
        ulwi.pblend = &BlendFuncAlpha;
        ulwi.dwFlags = ULW_ALPHA;
        ulwi.prcDirty = nullptr;
        UpdateLayeredWindowIndirect(hwnd, &ulwi);
    }

    LRESULT overlayWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CREATE:
            layoutFullscreen();
            // TODO 测试用
            danmaku_.emplace_back(L"测试文本 1", 70, 0xff'66ccff, 0xff'ffcc66);
            danmaku_.emplace_back(L"测试文本 2", 70, 0xff'ff0000, 0xff'0000ff);
            danmaku_.emplace_back(L"测试文本 3", 70, 0xff'00ff00, 0xff'0000ff);
            danmaku_.emplace_back(L"测试文本 4", 70, 0xff'0000ff, 0xff'00ffff);

            break;
        case WM_SIZE:
            width_ = LOWORD(lParam);
            height_ = HIWORD(lParam);
            recreateMemoryDC();
            break;
        case WM_DISPLAYCHANGE:
            layoutFullscreen();
            break;
        case WM_DESTROY:
            DeleteDC(cdc_);
            DeleteObject(bitmap_);
            oldObject_ = nullptr;
            break;
        }
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    overlayWindow &overlayWindow::create()
    {
        WNDCLASSW wc{};
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = baseWindow::wndProc<overlayWindow>;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        wc.lpszClassName = ClassName();

        if (!RegisterClassW(&wc))
        {
            debug::logOutput(L"[错误] 主窗口类注册错误\n");
            debug::logWinError(GetLastError());
            MessageBox(nullptr, L"（0001）主窗口：窗口类注册失败", L"出错了欸", MB_ICONERROR | MB_OK);
            return *this;
        }

        hwnd = CreateWindowExW(
            WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
            wc.lpszClassName, nullptr,
            WS_POPUP | WS_VISIBLE,
            0, 0, 400, 400,
            nullptr, nullptr, GetModuleHandle(nullptr), this);

        if (!hwnd)
        {
            debug::logOutput(L"[错误] 主窗口创建失败\n");
            debug::logWinError(GetLastError());
            MessageBox(nullptr, L"（0002）主窗口：创建窗口失败", L"出错了欸", MB_ICONERROR | MB_OK);
        }
        return *this;
    }
}