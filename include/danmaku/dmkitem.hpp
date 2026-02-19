#ifndef DANMAKU_DMKITEM_HPP
#define DANMAKU_DMKITEM_HPP

#include "dmkbmpcache.hpp"

#include <string>


namespace danmaku
{
    class DanmakuItem
    {
    private:
        Gdiplus::ARGB fillColor_{};
        Gdiplus::ARGB borderColor_{};
        std::wstring text_{};
        DanmakuBitmapCache::Bitmap bitmap_{};
        float width_{};
        float height_{};
        float emSize_{};
        float x_{};
        float speed_{}; // 像素/秒
    public:
        // 允许重复调用
        void rasterize();

        // 保留
        void invalidateCache();

        Gdiplus::Status draw(Gdiplus::GpGraphics *g, float x, float y);

        BOOL drawGdi(HDC dcDst, HDC cdc, float x, float y);

        DanmakuItem() = default;
        DanmakuItem(const DanmakuItem &) = delete;
        DanmakuItem &operator=(const DanmakuItem &) = delete;
        DanmakuItem(DanmakuItem &&) = default;
        DanmakuItem &operator=(DanmakuItem &&) = default;

        DanmakuItem(
            std::wstring_view text,
            float emSize,
            Gdiplus::ARGB fillColor,
            Gdiplus::ARGB borderColor) : fillColor_{fillColor},
                                         borderColor_{borderColor},
                                         text_{text},
                                         emSize_{emSize}
        {
        }

        ~DanmakuItem()
        {
            invalidateCache();
        }

        void setX(float x) { x_ = x; }
        void setSpeed(float speed) { speed_ = speed; }

        const std::wstring &getText() const { return text_; }
        Gdiplus::ARGB getFillColor() const { return fillColor_; }
        Gdiplus::ARGB getBorderColor() const { return borderColor_; }
        float getEmSize() const { return emSize_; }
        float getWidth() const { return width_; }
        float getHeight() const { return height_; }
        float getX() const { return x_; }
        float getSpeed() const { return speed_; }
    };
}

#endif // DANMAKU_DMKITEM_HPP