#include "functions/others.hpp"
#include <cstddef>

// 资源的安全释放
template <typename T>
void safeRelease(T **resourcePointer)
{
    // 检查是否为空
    if (*resourcePointer)
    {
        // 释放资源
        (*resourcePointer)->Release();
        // 设置为空
        *resourcePointer = NULL;
    }
}
