#ifndef DANMAKU_MAIN_WINDOW_HPP
#define DANMAKU_MAIN_WINDOW_HPP

#include <windows.h>
#include <string>
#include "windows/base.hpp"
#include "windows/elements.hpp"

namespace danmaku
{

    // 主窗口类
    class mainWindow : public baseWindow
    {
    private:
        // dm: danmaku main
        WNDCLASSEX dm_wc{};

    public:
        PCWSTR ClassName() const override { return dm_wc.lpszClassName; }
        // 处理窗口消息
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        // 构造函数和析构函数
        mainWindow() = default;
        // 使用默认析构函数
        ~mainWindow() = default;
        // 创建和显示函数
        mainWindow &create();
        // 重载 create 函数以接受窗口标题、宽度和高度参数
        mainWindow &create(std::wstring title, int width, int height);
        // 显示窗口
        mainWindow &show();
        /**
         * @brief 创建窗口元素的函数，接受一个元素信息结构体
         * @param[in,out] elements 一个元素信息结构体，包含元素类型、位置、大小、文本和额外信息
         * @return 返回主窗口对象的引用，以支持链式调用
         */
        mainWindow &createElements(elementInfo &elements);
        /**
         * @brief 创建窗口元素的函数，接受一个元素信息结构体数组和元素数量
         * @param[in,out] elements 一个元素信息结构体数组的首项指针，包含多个元素的信息
         * @param[in] count 元素信息数组成员数量
         * @return 返回主窗口对象的引用，以支持链式调用
         */
        mainWindow &createElements(elementInfo *elements, int count);
    };
}

#endif // DANMAKU_MAIN_WINDOW_HPP