#ifndef DANMAKU_OVERLAY_WINDOW_HPP
#define DANMAKU_OVERLAY_WINDOW_HPP

#include "base.hpp"
#include "elements.hpp"
#include "font.hpp"
#include "danmaku/dmkitem.hpp"

namespace danmaku
{
    class overlayWindow : public baseWindow
    {
    private:
        HDC cdc_{};
        HBITMAP bitmap_{};
        HGDIOBJ oldObject_{};
        Gdiplus::GpGraphics* graphics_{};
        int width_{}, height_{};// 客户区

        // TODO 弹幕管理
        std::vector<danmakuItem> danmaku_{};
        

        BOOL layoutFullscreen();

        void recreateMemoryDC();

    public:
        PCWSTR ClassName() const override { return L"Danmaku.WndCls.Overlay"; }

        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

        overlayWindow &create();

        baseWindow &show() override
        {
            ShowWindow(hwnd, SW_SHOW);
            return *this;
        }

        void paint();
    };
}

#endif // DANMAKU_OVERLAY_WINDOW_HPP