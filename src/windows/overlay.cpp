#include "windows/overlay.hpp"

using namespace Gdiplus::DllExports;

constexpr UINT_PTR TimerWorldTick = 1;      // 定时器ID
constexpr UINT TimerWorldTickInterval = 14; // 定时器间隔

namespace danmaku
{
    // 获取主监视器的句柄
    static HMONITOR getPrimaryMonitor()
    {
        // 主监视器句柄
        HMONITOR monitor{};
        // 枚举所有监视器，直到找到主监视器
        EnumDisplayMonitors(nullptr, nullptr, [](HMONITOR hMonitor, HDC, RECT *, LPARAM lParam) -> BOOL
                            {
            // 监视器信息变量
            MONITORINFO mi;
            mi.cbSize = sizeof(mi);
            // 获取监视器信息
            GetMonitorInfoW(hMonitor, &mi);
            // 如果该监视器是主监视器，则将其句柄存储在lParam指向的变量中，并停止枚举
            // 这里就是存入了monitor变量
            if (mi.dwFlags & MONITORINFOF_PRIMARY)
            {
                *(HMONITOR*)lParam = hMonitor;
                return FALSE;
            }
            else
                return TRUE; }, (LPARAM)&monitor);
        return monitor;
    }

    // 将窗口填充整个屏幕
    BOOL overlayWindow::layoutFullscreen()
    {
        // 获取主显示器句柄
        const auto mon = getPrimaryMonitor();
        if (!mon)
            return FALSE;
        // 获取主显示器的工作区信息
        MONITORINFO mi;
        mi.cbSize = sizeof(mi);
        // 如果成功获取到监视器信息，则将窗口位置和大小设置为覆盖整个工作区
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

    // 重新创建内存DC
    void overlayWindow::recreateMemoryDC()
    {
        // 获取窗口DC
        const auto dc = GetDC(hwnd);
        // 若内存DC未创建
        if (!cdc_)
        {
            cdc_ = CreateCompatibleDC(dc);
            // 设置背景模式透明，文字颜色浅黄色
            SetBkMode(cdc_, TRANSPARENT);
            SetTextColor(cdc_, 0xffcc66);
        }
        else
        {
            // 若DC已经创建，先恢复之前选入的位图对象
            SelectObject(cdc_, oldObject_);
        }
        // 删除旧的位图，准备创建新尺寸的位图
        DeleteObject(bitmap_);
        // 创建与窗口DC兼容的位图，大小为当前窗口的宽度和高度
        bitmap_ = CreateCompatibleBitmap(dc, width_, height_);
        // 将新位图选入内存DC，并保存旧的位图对象指针以便后续恢复
        oldObject_ = SelectObject(cdc_, bitmap_);

        GdipCreateFromHDC(cdc_, graphics_.addressOfClear());

        paint();
    }

    void overlayWindow::paint()
    {
        // 窗口客户区矩形
        const RECT rc{0, 0, width_, height_};
        // 清除
        FillRect(cdc_, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

        danmakuMgr_.draw(graphics_.get());

        // 设置分层窗口的混合参数（逐像素alpha）
        constexpr BLENDFUNCTION BlendFuncAlpha{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
        constexpr POINT SourcePoint{};    // 源DC中的起始点 (0,0)
        const SIZE size{width_, height_}; // 窗口尺寸

        // 初始化分层窗口更新结构体
        UPDATELAYEREDWINDOWINFO ulwi{};
        ulwi.cbSize = sizeof(ulwi);    // 结构体大小，用于版本识别
        ulwi.psize = &size;            // 指向窗口大小的指针
        ulwi.hdcSrc = cdc_;            // 源内存DC
        ulwi.pptSrc = &SourcePoint;    // 源DC中绘制的起点
        ulwi.pblend = &BlendFuncAlpha; // 混合函数参数
        ulwi.dwFlags = ULW_ALPHA;      // 使用 alpha 混合
        ulwi.prcDirty = nullptr;       // 整个窗口全部更新

        UpdateLayeredWindowIndirect(hwnd, &ulwi);
    }

    // 消息处理
    LRESULT overlayWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_TIMER:
            if (wParam == TimerWorldTick)
            {
                tick(TimerWorldTickInterval / 1000.0f); // 将毫秒转换为秒
            }
            break;
        case WM_CREATE:
            // TEMP 测试
            danmakuMgr_.setLineHeight(40);
            danmakuMgr_.setLineGap(10);
            danmakuMgr_.setDuration(5.0f);
            danmakuMgr_.setItemGap(10);
            danmakuMgr_.setSpeedFactor(1.0f);
            layoutFullscreen();
            SetTimer(hwnd, TimerWorldTick, TimerWorldTickInterval, nullptr);
            break;
        case WM_SIZE:
            // 更新窗口尺寸并重新创建内存DC
            width_ = LOWORD(lParam);
            height_ = HIWORD(lParam);
            danmakuMgr_.setScreenSize(width_, height_);
            danmakuMgr_.recalculateTracks();
            recreateMemoryDC();
            break;
        case WM_DISPLAYCHANGE:
            // 重新布局以适应显示设置的变化
            layoutFullscreen();
            break;
        case WM_DESTROY:
            // 清理资源：删除内存DC、位图和GDI+图形对象
            DeleteDC(cdc_);
            DeleteObject(bitmap_);
            oldObject_ = nullptr;
            break;
        }
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    // 创建窗口
    overlayWindow &overlayWindow::create()
    {
        WNDCLASSW wc{};
        // 窗口样式：当宽度或高度改变时重绘窗口
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = baseWindow::wndProc;
        wc.hInstance = GetModuleHandleW(nullptr);
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.lpszClassName = className();

        if (!RegisterClassW(&wc))
        {
            debug::logOutput(L"[错误] 主窗口类注册错误\n");
            debug::logWinError(GetLastError());
            MessageBoxW(nullptr, L"（0001）主窗口：窗口类注册失败", L"出错了欸", MB_ICONERROR | MB_OK);
            return *this;
        }

        hwnd = CreateWindowExW(
            WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
            wc.lpszClassName, nullptr,
            WS_POPUP | WS_VISIBLE,
            0, 0, 400, 400,
            nullptr, nullptr, GetModuleHandleW(nullptr), this);

        if (!hwnd)
        {
            debug::logOutput(L"[错误] 主窗口创建失败\n");
            debug::logWinError(GetLastError());
            MessageBoxW(nullptr, L"（0002）主窗口：创建窗口失败", L"出错了欸", MB_ICONERROR | MB_OK);
        }
        return *this;
    }
}