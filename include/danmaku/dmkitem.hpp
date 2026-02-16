#ifndef DANMAKU_DMKITEM_HPP
#define DANMAKU_DMKITEM_HPP

#include <windows.h>
#include <gdiplus.h>

#include <string>

namespace danmaku
{
    class danmakuItem
    {
    private:
        Gdiplus::ARGB fillColor_{};
        Gdiplus::ARGB borderColor_{};
        std::wstring text_{};
        Gdiplus::GpBitmap *bitmap_{};
        int width_{};
        int height_{};
        int emSize_{};

        void rasterize();

    public:
        void invalidateCache();

        Gdiplus::Status draw(Gdiplus::GpGraphics *g, int x, int y);

        danmakuItem() = default;
        danmakuItem(const danmakuItem &) = delete;
        danmakuItem &operator=(const danmakuItem &) = delete;

        danmakuItem(danmakuItem && x) noexcept;
        danmakuItem &operator=(danmakuItem && x) noexcept;

        danmakuItem(
            std::wstring_view text, int emSize,
            Gdiplus::ARGB fillColor, Gdiplus::ARGB borderColor) :
            fillColor_{fillColor}, borderColor_{borderColor},
            text_{text},  emSize_{emSize}
        {
        }

        ~danmakuItem();

        void setText(std::wstring_view text)
        {
            text_ = text;
            invalidateCache();
        }
        void setFillColor(Gdiplus::ARGB color)
        {
            fillColor_ = color;
            invalidateCache();
        }
        void setBorderColor(Gdiplus::ARGB color)
        {
            borderColor_ = color;
            invalidateCache();
        }
        void setEmSize(int emSize)
        {
            emSize_ = emSize;
            invalidateCache();
        }

        const std::wstring &getText() const { return text_; }
        Gdiplus::ARGB getFillColor() const { return fillColor_; }
        Gdiplus::ARGB getBorderColor() const { return borderColor_; }
        int getEmSize() const { return emSize_; }
    };
}

#endif // DANMAKU_DMKITEM_HPP