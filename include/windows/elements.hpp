#ifndef DANMAKU_ELEMENTS_HPP
#define DANMAKU_ELEMENTS_HPP
#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <stdexcept>
#include "windows/font.hpp"
#include "windows/extraElementInfo.hpp"
#include "debug.hpp"

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
        static UINT_PTR currentElementID;                       // 递增基值，初始0
        static std::unordered_map<UINT_PTR, element *> s_idMap; // ID -> 对象指针
        static std::vector<UINT_PTR> s_freeIds;                 // 可重用的ID列表
        static std::mutex s_mutex;                              // 保护静态资源的互斥量
        static void idTransfer(UINT_PTR id, element *newOwner); // ID所有权转移函数

        // 静态辅助函数：分配一个新ID（线程安全）
        static UINT_PTR allocateID(element *elem);
        // 静态辅助函数：释放一个ID（线程安全）
        static void releaseID(UINT_PTR id, element *obj);

        UINT_PTR elementID = 0;      // 存储元素的唯一标识符，以便在事件处理时区分不同的元素
        HWND parentHwnd = nullptr;   // 存储父窗口的句柄，以便在创建元素时指定父窗口
        HWND hwnd = nullptr;         // 存储元素的窗口句柄，以便后续操作使用
        HFONT elementFont = nullptr; // 存储元素的字体对象，以便后续操作使用
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
            : elementID(allocateID(this)), parentHwnd(parentHwnd), elementFont(textFont.getHandle()),
              type(type), position(position), text(std::move(text)), extra(extra) {}
        // 析构函数
        ~element()
        {
            if (elementID != 0)
                releaseID(elementID, this);
        }
        element(const element &) = delete;            // 禁止复制构造
        element &operator=(const element &) = delete; // 禁止复制赋值
        element(element &&other) noexcept
            : elementID(other.elementID), parentHwnd(other.parentHwnd), hwnd(other.hwnd),
              elementFont(other.elementFont), type(other.type), position(other.position),
              text(std::move(other.text)), extra(other.extra)
        {
            other.elementID = 0; // 转移所有权后重置源对象的ID，防止析构时误释放
            other.hwnd = nullptr;
            other.elementFont = nullptr;
        }

        element &resetFont(HFONT font);
        HRESULT procMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        template <typename... Args>
        friend void createElements(Args &...args);

        // 访问ID
        UINT_PTR getID() const { return elementID; }
        // 友元声明（实际可能无需访问私有成员，保留以符合接口）
        friend element &searchID(UINT_PTR id);
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

    element &searchID(UINT_PTR id);
}
#endif // DANMAKU_ELEMENTS_HPP