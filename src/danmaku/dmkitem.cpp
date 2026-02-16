#include "danmaku/dmkitem.hpp"

#include <assert.h>

using namespace Gdiplus::DllExports;

namespace danmaku
{
    void danmakuItem::rasterize()
    {
        assert(!bitmap_);

        // TODO 拆离字体逻辑，对接全局字体管理

        Gdiplus::GpStringFormat *strFmt;
        GdipCreateStringFormat(0, LANG_NEUTRAL, &strFmt);
        Gdiplus::GpFontFamily *fontFamily;
        GdipCreateFontFamilyFromName(L"微软雅黑", nullptr, &fontFamily);

        Gdiplus::GpPath *path;
        GdipCreatePath(Gdiplus::FillModeAlternate, &path);

        const Gdiplus::RectF InfRect{0, 0, 65536.f, 65536.f};
        GdipAddPathString(
            path,
            text_.data(), (int)text_.size(),
            fontFamily,
            Gdiplus::FontStyleRegular,
            emSize_,
            &InfRect,
            strFmt);

        Gdiplus::GpPen *pen;
        GdipCreatePen1(borderColor_, 1.f, Gdiplus::UnitPixel, &pen);
        Gdiplus::GpSolidFill *brush;
        GdipCreateSolidFill(fillColor_, &brush);

        Gdiplus::Rect pathRect;
        GdipGetPathWorldBoundsI(path, &pathRect, nullptr, pen);

        GdipCreateBitmapFromScan0(
            pathRect.Width, pathRect.Height,
            0, PixelFormat32bppPARGB, nullptr, &bitmap_);
        {
            Gdiplus::GpGraphics *g;
            GdipGetImageGraphicsContext(bitmap_, &g);

            GdipSetSmoothingMode(g, Gdiplus::SmoothingModeAntiAlias);

            Gdiplus::GpMatrix *mat;
            GdipCreateMatrix(&mat);
            GdipTranslateMatrix(mat, -pathRect.X, -pathRect.Y,
                                Gdiplus::MatrixOrderAppend);

            GdipSetWorldTransform(g, mat);

            GdipFillPath(g, brush, path);
            GdipDrawPath(g, pen, path);

            GdipDeleteGraphics(g);
            GdipDeleteMatrix(mat);
        }

        GdipDeletePen(pen);
        GdipDeleteBrush(brush);
        GdipDeletePath(path);
        GdipDeleteFontFamily(fontFamily);
        GdipDeleteStringFormat(strFmt);
    }

    Gdiplus::Status danmakuItem::draw(Gdiplus::GpGraphics *g, int x, int y)
    {
        if (!bitmap_)
            rasterize();
        return GdipDrawImageI(g, bitmap_, x, y);
    }

    danmakuItem::danmakuItem(danmakuItem &&x) noexcept
    {
        if (this == &x)
            return;

        fillColor_ = x.fillColor_;
        borderColor_ = x.borderColor_;
        text_ = std::move(x.text_);
        bitmap_ = x.bitmap_;
        width_ = x.width_;
        height_ = x.height_;
        emSize_ = x.emSize_;

        x.bitmap_ = nullptr;
    }

    danmakuItem &danmakuItem::operator=(danmakuItem &&x) noexcept
    {
        if (this == &x)
            return *this;

        if (bitmap_)
        {
            GdipDisposeImage(bitmap_);
            bitmap_ = nullptr;
        }
        fillColor_ = x.fillColor_;
        borderColor_ = x.borderColor_;
        text_ = std::move(x.text_);
        bitmap_ = x.bitmap_;
        width_ = x.width_;
        height_ = x.height_;
        emSize_ = x.emSize_;

        x.bitmap_ = nullptr;

        return *this;
    }

    danmakuItem::~danmakuItem()
    {
        if (bitmap_)
            GdipDisposeImage(bitmap_);
    }

    void danmakuItem::invalidateCache()
    {
        if (bitmap_)
        {
            GdipDisposeImage(bitmap_);
            bitmap_ = nullptr;
        }
    }
}