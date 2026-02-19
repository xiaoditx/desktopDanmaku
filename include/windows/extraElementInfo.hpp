#ifndef extraElementInfo_HPP
#define extraElementInfo_HPP
#include "windows.h"
#include <functional>

namespace danmaku
{
    struct LabelExtraInfo
    {
        // 颜色
        COLORREF textColor = RGB(0, 0, 0);             // 默认黑色
        COLORREF backgroundColor = RGB(255, 255, 255); // 默认白色
    };
    enum class ButtonProcType
    {
        click, // 姑且只用这一个，下面的以后再说
        hover, // 好吧其实以后也不一定能用得到
        leave
    };
    struct ButtonExtraInfo
    {
        std::function<void()> clickProc = nullptr; // 点击事件处理函数
        std::function<void()> hoverProc = nullptr; // 鼠标悬停事件
        std::function<void()> leaveProc = nullptr; // 鼠标离开事件
    };
}
#endif // extraElementInfo_HPP