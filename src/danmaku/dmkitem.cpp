#include "danmaku/dmkitem.hpp"

#include <assert.h>

using namespace Gdiplus::DllExports;

namespace danmaku
{
    void danmakuItem::rasterize()
    {
        if (bitmap_.get())
            return;

        // TODO 拆离字体逻辑，对接全局字体管理
        // TODO 允许自定义字体属性（如字体名称、加粗、斜体等）

        // 创建字符串格式对象（使用默认格式）
        GpPtr<Gdiplus::GpStringFormat> strFmt;
        GdipCreateStringFormat(0, LANG_NEUTRAL, &strFmt);

        // 创建字体族对象
        GpPtr<Gdiplus::GpFontFamily> fontFamily;
        GdipCreateFontFamilyFromName(L"微软雅黑", nullptr, &fontFamily);

        // 创建路径对象，用于容纳文本的几何轮廓
        GpPtr<Gdiplus::GpPath> path;
        GdipCreatePath(Gdiplus::FillModeAlternate, &path);

        // 定义一个极大的矩形区域（65536x65536），确保文本能完全放入
        const Gdiplus::RectF InfRect{0, 0, 65536.f, 65536.f};

        // 将文本字符串转换为路径，指定字体、样式、大小等
        GdipAddPathString(
            path.get(),
            text_.data(), (int)text_.size(),
            fontFamily.get(),
            Gdiplus::FontStyleRegular,
            emSize_,
            &InfRect,
            strFmt.get());

        // 创建画笔（用于绘制边框）和画刷（用于填充文本）
        GpPtr<Gdiplus::GpPen> pen;
        GdipCreatePen1(borderColor_, 2.f, Gdiplus::UnitPixel, &pen);
        GpPtr<Gdiplus::GpSolidFill> brush;
        GdipCreateSolidFill(fillColor_, &brush);

        // 获取路径的世界边界矩形（考虑到边框宽度，传入画笔）
        Gdiplus::Rect pathRect;
        GdipGetPathWorldBoundsI(path.get(), &pathRect, nullptr, pen.get());
        width_ = pathRect.Width;
        height_ = pathRect.Height;

        // 创建与边界矩形等大的位图（32位PARGB格式，支持透明通道）
        GdipCreateBitmapFromScan0(
            pathRect.Width, pathRect.Height,
            0,
            PixelFormat32bppPARGB,
            nullptr,
            &bitmap_);

        // 从位图获取图形上下文，用于绘制文本
        GpPtr<Gdiplus::GpGraphics> g;
        GdipGetImageGraphicsContext(bitmap_.get(), &g);

        // 开启抗锯齿，使文本边缘平滑
        GdipSetSmoothingMode(g.get(), Gdiplus::SmoothingModeAntiAlias);

        // 创建矩阵，用于将世界变换平移，使路径的左上角对齐到(0,0)
        GpPtr<Gdiplus::GpMatrix> mat;
        GdipCreateMatrix(&mat);
        GdipTranslateMatrix(mat.get(), -pathRect.X, -pathRect.Y,
                            Gdiplus::MatrixOrderAppend);

        // 应用矩阵到图形上下文的世界变换
        GdipSetWorldTransform(g.get(), mat.get());

        // 填充路径（文本主体）并绘制路径边框
        GdipDrawPath(g.get(), pen.get(), path.get());
        GdipFillPath(g.get(), brush.get(), path.get());
    }

    // 在指定的图形上下文中绘制弹幕位图
    Gdiplus::Status danmakuItem::draw(Gdiplus::GpGraphics *g, float x, float y)
    {
        // 若尚未光栅化，则立即执行
        if (!bitmap_.get())
            rasterize();
        // 在位图的指定位置绘制图像
        return GdipDrawImage(g, bitmap_.get(), x, y);
    }

    void danmakuItem::invalidateCache()
    {
        bitmap_.clear();
    }
}