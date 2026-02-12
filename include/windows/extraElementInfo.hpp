#ifndef extraElementInfo_HPP
#define extraElementInfo_HPP
#include "windows.h"

namespace danmaku
{
    struct labelExtraInfo
    {
        // 颜色
        COLORREF textColor = RGB(0, 0, 0);             // 默认黑色
        COLORREF backgroundColor = RGB(255, 255, 255); // 默认白色
    };
}
#endif // extraElementInfo_HPP