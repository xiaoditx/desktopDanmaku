#include "pch.hpp"
#include "functions/str.hpp"

/**
 * 检查一个码点是否是高位代理项
 */
bool isHighSurrogate(wchar_t c) {
    return c >= 0xD800 && c <= 0xDBFF;
}

/**
 * 检查一个码点是否是低位代理项
 */
bool isLowSurrogate(wchar_t c) {
    return c >= 0xDC00 && c <= 0xDFFF;
}

/**
 * 检查一个码点是否是变体选择符
 */
bool isVariationSelector(wchar_t c) {
    return c >= 0xFE00 && c <= 0xFE0F;
}

/**
 * 检查一个码点是否是组合标记
 */
bool isCombiningMark(wchar_t c) {
    return (c >= 0x0300 && c <= 0x036F) || // 组合变音标记
           (c >= 0x1DC0 && c <= 0x1DFF) || // 组合变音标记补充
           (c >= 0x20D0 && c <= 0x20FF) || // 组合变音标记扩展
           (c >= 0xFE20 && c <= 0xFE2F);   // 组合半标记
}

/**
 * 统计字符串的观感字数
 */
size_t countVisibleCharacters(std::wstring_view str) {
    size_t count = 0;
    size_t length = str.length();
    
    for (size_t i = 0; i < length; ++i) {
        wchar_t c = str[i];
        
        // 跳过变体选择符和组合标记，它们不增加观感字数
        if (isVariationSelector(c) || isCombiningMark(c)) {
            // 不增加计数，继续处理下一个字符
            continue;
        }
        
        // 检查是否是代理对（用于表示超出U+FFFF的字符，如emoji）
        if (isHighSurrogate(c)) {
            // 如果下一个字符是低位代理项，则跳过它，将两个视为一个
            if (i + 1 < length && isLowSurrogate(str[i + 1])) {
                ++i; // 跳过低位代理项
            }
            ++count;
        }
        // 其他所有字符（包括汉字、字母、普通emoji等）都算一个字
        else {
            ++count;
        }
    }
    
    return count;
}

/**
 * 统计字符串的观感字数（重载，支持wchar_t*）
 */
size_t countVisibleCharacters(const wchar_t* str) {
    if (!str) {
        return 0;
    }
    
    std::wstring s(str);
    return countVisibleCharacters(s);
}