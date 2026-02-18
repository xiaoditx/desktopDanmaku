#ifndef GPPTR_HPP
#define GPPTR_HPP

#include <gdiplus.h>

template <class T>
class GpPtr
{
private:
    T *ptr_{};

    void clearInternal() noexcept
    {
        if constexpr (std::is_same_v<T, Gdiplus::GpGraphics>)
            Gdiplus::DllExports::GdipDeleteGraphics(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpBitmap>)
            Gdiplus::DllExports::GdipDisposeImage(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpPen>)
            Gdiplus::DllExports::GdipDeletePen(ptr_);
        else if constexpr (std::is_base_of_v<Gdiplus::GpBrush, T>)
            Gdiplus::DllExports::GdipDeleteBrush(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpPath>)
            Gdiplus::DllExports::GdipDeletePath(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpFontFamily>)
            Gdiplus::DllExports::GdipDeleteFontFamily(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpStringFormat>)
            Gdiplus::DllExports::GdipDeleteStringFormat(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpMatrix>)
            Gdiplus::DllExports::GdipDeleteMatrix(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpFont>)
            Gdiplus::DllExports::GdipDeleteFont(ptr_);
        else if constexpr (std::is_same_v<T, Gdiplus::GpRegion>)
            Gdiplus::DllExports::GdipDeleteRegion(ptr_);
        else
            static_assert(false, "Unsupported GDI+ type");
    }

public:
    constexpr GpPtr() = default;
    explicit constexpr GpPtr(T *ptr) noexcept : ptr_(ptr) {}

    GpPtr(const GpPtr &) = delete;
    GpPtr &operator=(const GpPtr &) = delete;

    GpPtr(GpPtr &&x) noexcept { std::swap(ptr_, x.ptr_); }
    GpPtr &operator=(GpPtr &&x) noexcept
    {
        std::swap(ptr_, x.ptr_);
        return *this;
    }

    ~GpPtr()
    {
        if (ptr_)
            clearInternal();
    }

    [[nodiscard]] constexpr T *get() const noexcept { return ptr_; }

    [[nodiscard]] constexpr T **operator&() noexcept { return &ptr_; }
    [[nodiscard]] constexpr T *const *operator&() const noexcept { return &ptr_; }

    [[nodiscard]] constexpr T **addressOf() noexcept { return &ptr_; }
    [[nodiscard]] constexpr T *const *addressOf() const noexcept { return &ptr_; }

    [[nodiscard]] constexpr T **addressOfClear() noexcept
    {
        clear();
        return &ptr_;
    }

    void clear() noexcept
    {
        if (ptr_)
        {
            clearInternal();
            ptr_ = nullptr;
        }
    }

    void attach(T *ptr) noexcept
    {
        clear();
        ptr_ = ptr;
    }

    [[nodiscard]] constexpr T *detach() noexcept
    {
        const auto temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
};

#endif // GPPTR_HPP