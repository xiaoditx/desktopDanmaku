#include "pch.hpp"
#include "danmaku/dmkbmpcache.hpp"

namespace danmaku
{
    BOOL DanmakuBitmapCache::allocate(int width, int height, Bitmap &outBmp)
    {
        {// 查找一个尺寸大于等于请求尺寸的缓存
            SrwExclusiveGuard _{lock_};
            for (auto it = cache_.begin(); it != cache_.end(); )
            {
                if (it->width >= width && it->height >= height)
                {
                    outBmp = std::move(*it);
                    outBmp.touch = version_;
                    ZeroMemory(outBmp.dibData, outBmp.width * outBmp.height * 4);
                    cache_.erase(it);
                    return TRUE;
                }
                else
                {
                    if (it->touch > version_) // 可能溢出
                        it->touch = version_;
                    
                    if (version_ - it->touch > 600)
                        it = cache_.erase(it);
                    else
                        ++it;
                }
            }
        }

        // 没有合适的缓存，创建新的位图

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
        outBmp.touch = version_;
        return TRUE;
    }

    void DanmakuBitmapCache::free(Bitmap &&bmp)
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