#ifndef STR_HPP
#define STR_HPP

#include "pch.hpp"

/**
 * 统计字符串的观感字数
 * 规则：
 * - 一个汉字算一个字
 * - 一个字母算一个字
 * - 一个组合emoji算一个字（如国旗表情）
 *
 * @param str 要统计的宽字符串
 * @return 观感字数
 */
size_t countVisibleCharacters(std::wstring_view str);

/**
 * 统计字符串的观感字数（重载，支持wchar_t*）
 *
 * @param str 要统计的宽字符串指针
 * @return 观感字数
 */
size_t countVisibleCharacters(const wchar_t *str);

/**
 * @brief 将十六进制字符串转换为ARGB颜色值
 * @param hexStr: 输入的十六进制字符串，可以带有前缀（# 或 0x），也可以不带前缀
 * @return 转换后的ARGB颜色值，如果输入无效则返回默认颜色（黑色不透明或白色不透明）
 */
Gdiplus::ARGB hexStringToArgb(const std::wstring &hexStr);

#endif