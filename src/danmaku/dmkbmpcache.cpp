#include "danmaku/dmkbmpcache.hpp"

#include <assert.h>

using namespace Gdiplus::DllExports;

namespace danmaku
{
    BOOL danmakuBitmapCache::allocate(int width, int height, Bitmap &outBmp)
    {
        {
            SrwExclusiveGuard guard(lock_);
            for (auto it = cache_.begin(); it != cache_.end(); ++it)
            {
                if (it->width >= width && it->height >= height)
                {
                    outBmp = std::move(*it);
                    cache_.erase(it);
                    return TRUE;
                }
            }
        }

        Bitmap bmp{};

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmp.dib = CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS,
                                   &bmp.dibData, nullptr, 0);

        GdipCreateBitmapFromScan0(
            width, height,
            width * 4,
            PixelFormat32bppPARGB,
            (BYTE *)bmp.dibData,
            &bmp.bitmap);

        outBmp = std::move(bmp);
        outBmp.width = width;
        outBmp.height = height;
        return TRUE;
    }

    void danmakuBitmapCache::free(Bitmap &&bmp)
    {
        if (!bmp.bitmap.get())
        {
            assert(!bmp.dib);
            assert(!bmp.dibData);
            return;
        }
        SrwExclusiveGuard guard(lock_);
        cache_.emplace_back(std::move(bmp));
    }
}