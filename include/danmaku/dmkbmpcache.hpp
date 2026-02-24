#ifndef DANMAKU_DMKBMPCACHE_HPP
#define DANMAKU_DMKBMPCACHE_HPP

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
            UINT touch{};

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
                std::swap(touch, x.touch);
            }
            Bitmap &operator=(Bitmap &&x) noexcept
            {
                std::swap(dib, x.dib);
                std::swap(dibData, x.dibData);
                std::swap(bitmap, x.bitmap);
                std::swap(width, x.width);
                std::swap(height, x.height);
                std::swap(touch, x.touch);
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
        static inline DanmakuBitmapCache *instance_{};

        std::vector<Bitmap> cache_{};
        SrwLock lock_{};
        UINT version_{};

        DanmakuBitmapCache() = default;

    public:
        static DanmakuBitmapCache &instance()
        {
            return *instance_;
        }

        // 必须与GDI+全局生命周期同步

        static void startup() { instance_ = new DanmakuBitmapCache{}; }
        static void shutdown()
        {
            delete instance_;
            instance_ = nullptr;
        }

        BOOL allocate(int width, int height, Bitmap &outBmp);

        // 放回空闲列表以重用，可以传递空Bitmap对象
        void free(Bitmap &&bmp);

        void tick()
        {
            SrwExclusiveGuard _{lock_};
            ++version_;
        }
    };
}

#endif // DANMAKU_DMKBMPCACHE_HPP