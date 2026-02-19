#include <windows.h>

#include <vector>

#include "functions/gpptr.hpp"
#include "functions/srwlk.hpp"

namespace danmaku
{
    class DanmakuBitmapCache
    {
    public:
        struct Bitmap
        {
            HBITMAP dib{};
            void *dibData{};
            GpPtr<Gdiplus::GpBitmap> bitmap{};
            int width{};
            int height{};

            Bitmap() = default;
            Bitmap(const Bitmap &) = delete;
            Bitmap &operator=(const Bitmap &) = delete;
            Bitmap(Bitmap &&x) noexcept
            {
                std::swap(dib, x.dib);
                std::swap(dibData, x.dibData);
                std::swap(bitmap, x.bitmap);
                std::swap(width, x.width);
                std::swap(height, x.height);
            }
            Bitmap &operator=(Bitmap &&x) noexcept
            {
                std::swap(dib, x.dib);
                std::swap(dibData, x.dibData);
                std::swap(bitmap, x.bitmap);
                std::swap(width, x.width);
                std::swap(height, x.height);
                return *this;
            }

            ~Bitmap()
            {
                DeleteObject(dib);
            }

            void clear()
            {
                DeleteObject(dib);
                dib = nullptr;
                dibData = nullptr;
                bitmap.clear();
                width = 0;
                height = 0;
            }
    };
    private:
        std::vector<Bitmap> cache_{};
        SrwLock lock_{};

        DanmakuBitmapCache() = default;
    public:
        static DanmakuBitmapCache &instance()
        {
            static DanmakuBitmapCache cache{};
            return cache;
        }

        BOOL allocate(int width, int height, Bitmap &outBmp);

        void free(Bitmap &&bmp);
    };
}