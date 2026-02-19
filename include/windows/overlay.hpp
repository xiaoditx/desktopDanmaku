#ifndef DANMAKU_OVERLAY_WINDOW_HPP
#define DANMAKU_OVERLAY_WINDOW_HPP

#include "base.hpp"
#include "elements.hpp"
#include "font.hpp"
#include "danmaku/dmkmgr.hpp"

namespace danmaku
{
    // overlay窗口覆盖整个屏幕，用于显示弹幕
    class OverlayWindow : public BaseWindow
    {
    private:
        // 内存DC，用于双缓冲绘制
        HDC cdc_{};
        // 位图传输时用
        HDC cdcTemp_{};
        // 内存位图，用于双缓冲绘制
        HBITMAP bitmap_{};
        // 选择的旧位图对象，用于恢复DC状态
        HGDIOBJ oldObject_{};
        int width_{}, height_{}; // 客户区

        DanmakuManager danmakuMgr_;

        BOOL layoutFullscreen();

        void recreateMemoryDC();
    public:
        PCWSTR className() const override { return L"Danmaku.WndCls.Overlay"; }

        LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

        OverlayWindow &create();

        BaseWindow &show() override
        {
            ShowWindow(hwnd, SW_SHOW);
            return *this;
        }

        void paint();

        // 添加弹幕的方法
        void addDanmaku(const std::wstring &text, float emSize, Gdiplus::ARGB fillColor, Gdiplus::ARGB borderColor)
        {
            danmakuMgr_.addDanmaku(DanmakuItem{text, emSize, fillColor, borderColor});
            paint();
        }

        void tick(float dt)
        {
            danmakuMgr_.tick(dt);
            paint();
        }
    };
}

#endif // DANMAKU_OVERLAY_WINDOW_HPP