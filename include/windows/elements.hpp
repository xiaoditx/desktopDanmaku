#ifndef DANMAKU_ELEMENTS_HPP
#define DANMAKU_ELEMENTS_HPP
#include <windows.h>
#include <string>
#include "windows/font.hpp"

namespace danmaku
{
    // elements枚举定义了窗口元素的类型，包括按钮、编辑框和标签等
    enum class elements : int
    {
        button,
        edit,
        label
    };
    // elementInfo封装了窗口元素的信息，包括类型、位置、大小、文本内容、字体和额外信息等
    class elementInfo
    {
    private:
        HWND hwnd = nullptr;         // 存储元素的窗口句柄，以便后续操作使用
        HFONT elementFont = nullptr; // 存储元素的字体对象，以便后续操作使用
    public:
        elements type;
        int x;
        int y;
        int width;
        int height;
        std::wstring text;
        void *extra;             // 预留字段，用于存储额外信息
        friend class mainWindow; // 允许窗口访问 elementInfo 的私有成员
        elementInfo() = default;
        elementInfo(elements type, int x, int y,
                    int width, int height, std::wstring text, const font &textFont = font(),
                    void *extra = nullptr)
            : elementFont(textFont.getHandle()), type(type), x(x), y(y), width(width), height(height), text(std::move(text)), extra(extra) {}
        elementInfo &resetFont(HFONT font);
    };
}
#endif // DANMAKU_ELEMENTS_HPP