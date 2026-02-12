#ifndef DANMAKU_ELEMENTS_HPP
#define DANMAKU_ELEMENTS_HPP
#include <windows.h>
#include <string>
#include "windows/font.hpp"
#include "windows/extraElementInfo.hpp"

namespace danmaku
{
    struct rect
    {
        int x;
        int y;
        int width;
        int height;
    };
    // elementType枚举定义了窗口元素的类型，包括按钮、编辑框和标签等
    enum class elementType : int
    {
        button,
        edit,
        label
    };
    // element封装了窗口元素的信息，包括类型、位置、大小、文本内容、字体和额外信息等
    class element
    {
    private:
        static UINT_PTR currentElementID; // 静态成员变量，用于生成唯一的元素ID，初始值为0
        UINT_PTR elementID = 0;           // 存储元素的唯一标识符，以便在事件处理时区分不同的元素
        HWND parentHwnd = nullptr;        // 存储父窗口的句柄，以便在创建元素时指定父窗口
        HWND hwnd = nullptr;              // 存储元素的窗口句柄，以便后续操作使用
        HFONT elementFont = nullptr;      // 存储元素的字体对象，以便后续操作使用
        elementType type;
        rect position;
        std::wstring text;
        void *extra; // 预留字段，用于存储额外信息

    protected:
        element &create();

    public:
        element() = default;
        element(HWND parentHwnd, elementType type, rect position, std::wstring text, const font &textFont = font(),
                void *extra = nullptr)
            : parentHwnd(parentHwnd), elementFont(textFont.getHandle()),
              type(type), position(position), text(std::move(text)), extra(extra) {}
        element &resetFont(HFONT font);
        template <typename... Args>
        friend void createElements(Args &...args);
    };

    // 可变参数模板函数，参数为派生类对象的常量左值引用
    template <typename... Args>
    void createElements(Args &...args)
    {
        // 编译期：检查每个参数类型是否公有继承自 element
        static_assert((std::is_base_of_v<element, Args> && ...),
                      "All arguments must be derived from element");
        // 使用折叠表达式遍历所有参数
        (args.create(), ...);
    }
}
#endif // DANMAKU_ELEMENTS_HPP