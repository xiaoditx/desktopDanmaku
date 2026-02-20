#include "pch.hpp"
#include "windows/overlay.hpp"
#include "functions/randnum.hpp"

using namespace Gdiplus::DllExports;

constexpr UINT TimerWorldTickInterval = 14;    // 定时器间隔
constexpr UINT TimerWorldTickMinInterval = 12; // TimerWorldTickInterval减去误差

namespace danmaku
{
    static BOOL CALLBACK monitorEnumProc(
        HMONITOR monitor, HDC, LPRECT, LPARAM param)
    {
        MONITORINFO mi;
        mi.cbSize = sizeof(mi);
        if (GetMonitorInfoW(monitor, &mi) && (mi.dwFlags & MONITORINFOF_PRIMARY))
        {
            *(HMONITOR *)param = monitor;
            return FALSE;
        }
        return TRUE;
    }
    // 获取主监视器的句柄
    static HMONITOR getPrimaryMonitor()
    {
        // 主监视器句柄
        HMONITOR monitor{};
        // 枚举所有监视器，直到找到主监视器
        EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, (LPARAM)&monitor);
        return monitor;
    }

    // 将窗口填充整个屏幕
    BOOL OverlayWindow::layoutFullscreen()
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
    void OverlayWindow::recreateMemoryDC()
    {
        // 获取窗口DC
        const auto dc = GetDC(hwnd);
        // 若内存DC未创建
        if (!cdc_)
            cdc_ = CreateCompatibleDC(dc);
        else // 挤掉将要删除的位图
            SelectObject(cdc_, oldObject_);

        if (!cdcTemp_)
            cdcTemp_ = CreateCompatibleDC(dc);
        // 删除旧的位图，准备创建新尺寸的位图
        DeleteObject(bitmap_);
        // 创建与窗口DC兼容的位图，大小为当前窗口的宽度和高度
        bitmap_ = CreateCompatibleBitmap(dc, width_, height_);
        // 将新位图选入内存DC，并保存旧的位图对象指针以便后续恢复
        oldObject_ = SelectObject(cdc_, bitmap_);

        paint();
    }

    DWORD CALLBACK OverlayWindow::timerThread(void* param)
    {
        const auto self = (OverlayWindow *)param;
        const auto timer = CreateWaitableTimerW(nullptr, FALSE, nullptr);

        ULONGLONG lastTick = GetTickCount64(); // 上一帧的开始时刻
        while (!self->timerThreadExit_)
        {
            const auto now = GetTickCount64(); // 帧开始时刻
            if (const auto param = WPARAM(now - lastTick))
                SendMessageW(self->hwnd, MessageClockTick, param, 0);

            const auto delta = GetTickCount64() - lastTick; // SendMessageW消耗的时间
            lastTick = now;
            if (delta < TimerWorldTickMinInterval) // 延时以消耗剩余时间
            {
                LARGE_INTEGER dueTime;
                dueTime.QuadPart = -((TimerWorldTickMinInterval - delta) * 10000ll);
                SetWaitableTimer(timer, &dueTime, 0, nullptr, nullptr, FALSE);
                WaitForSingleObject(timer, INFINITE);
            }
        }
        CloseHandle(timer);
        return 0;
    }

    void OverlayWindow::paint()
    {
        const auto &dirtyRect = danmakuMgr_.getDirtyRect();
        if (IsRectEmpty(&dirtyRect))
            return;
        // 清除
        FillRect(cdc_, &dirtyRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        danmakuMgr_.drawGdi(cdc_, cdcTemp_);

#ifdef _DEBUG
        // 脏矩形参考边框
        if (0)
        {
            GpPtr<Gdiplus::GpGraphics> g;
            GdipCreateFromHDC(cdc_, &g);
            GpPtr<Gdiplus::GpPen> pen;
            GdipCreatePen1(0xff'000000 | random::getInt(0, 0xffffff),
                           2.f, Gdiplus::UnitPixel, &pen);
            GdipSetPenMode(pen.get(), Gdiplus::PenAlignmentInset);
            GdipDrawRectangle(g.get(), pen.get(),
                              dirtyRect.left,
                              dirtyRect.top,
                              dirtyRect.right - dirtyRect.left,
                              dirtyRect.bottom - dirtyRect.top);
        }
#endif // _DEBUG

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
        // 裁剪脏矩形到DC范围
        const RECT clientRect{0, 0, width_, height_};
        RECT realRect;
        IntersectRect(&realRect, &dirtyRect, &clientRect);
        ulwi.prcDirty = &realRect;

        UpdateLayeredWindowIndirect(hwnd, &ulwi);
    }

    // 消息处理
    LRESULT OverlayWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case MessageClockTick:
            tick(wParam / 1000.f);
            break;
        case WM_CREATE:
            // TEMP 测试
            danmakuMgr_.setLineHeight(40);
            danmakuMgr_.setLineGap(10);
            danmakuMgr_.setDuration(5.f);
            danmakuMgr_.setItemGap(10);
            danmakuMgr_.setSpeedFactor(1.f);
            layoutFullscreen();
            // 启动定时线程
            assert(!timerThreadHandle_);
            timerThreadExit_ = FALSE;
            timerThreadHandle_ = CreateThread(nullptr, 0, timerThread, this, 0, nullptr);
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
            // 终止定时线程
            timerThreadExit_ = TRUE;
            WaitForSingleObject(timerThreadHandle_, INFINITE);
            CloseHandle(timerThreadHandle_);
            timerThreadHandle_ = nullptr;
            // 清理资源：删除内存DC、位图和GDI+图形对象
            DeleteDC(cdc_);
            cdc_ = nullptr;
            DeleteDC(cdcTemp_);
            cdcTemp_ = nullptr;
            DeleteObject(bitmap_);
            bitmap_ = nullptr;
            oldObject_ = nullptr;
            break;
        }
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    // 创建窗口
    OverlayWindow &OverlayWindow::create()
    {
        WNDCLASSW wc{};
        // 窗口样式：当宽度或高度改变时重绘窗口
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = BaseWindow::wndProc;
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