#ifndef STR_HPP
#define STR_HPP

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
size_t countVisibleCharacters(const wchar_t* str);

#endif