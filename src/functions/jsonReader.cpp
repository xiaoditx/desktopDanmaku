#include "yyjson/yyjson.h"
#include "pch.hpp"
#include <locale>
#include <cstdint>
#include <stdexcept>
#include <windows.h> // 用于 MultiByteToWideChar
#include "danmaku/dmkitem.hpp"

/*
json文件示例：

[
  {
    "fillColor": 4278190335,
    "borderColor": 4294901760,
    "text": "Hello",
    "time": 1234567890
  },
  {
    "fillColor": 4281545523,
    "borderColor": 4286611584,
    "text": "World",
    "time": 9876543210
  }
]

*/

namespace danmaku
{
    // wstring 转 UTF-8 string（注：codecvt_utf8在C++17被标记为弃用，理论上讲其实不应该这样写）
    std::string wstring_to_utf8(const std::wstring &wstr)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }

    // 将 UTF-8 字符串转换为 std::wstring (UTF-16)
    static std::wstring Utf8ToWideString(const char *utf8)
    {
        if (!utf8)
            return {};
        int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
        if (len <= 0)
            return {};
        std::wstring result(len - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &result[0], len);
        return result;
    }

    // 将 std::wstring(UTF - 16) 转换为 UTF - 8 编码的 std::string
    static std::string WideStringToUtf8(const std::wstring &wstr)
    {
        if (wstr.empty())
            return {};

        int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (len <= 0)
        {
            throw std::runtime_error("Failed to convert wide string to UTF-8");
        }

        std::string result(len - 1, '\0'); // 排除末尾的 null 终止符
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], len, nullptr, nullptr);
        return result;
    }

    std::vector<DanmakuItem> ReadDanmakuArrayFromJsonFile(const std::wstring &filename)
    {
        // 将宽字符串路径转换为 UTF-8
        std::string utf8Path = WideStringToUtf8(filename);

        // 使用 yyjson 读取文件
        yyjson_doc *doc = yyjson_read_file(utf8Path.c_str(), 0, nullptr, nullptr);
        if (!doc)
        {
            throw std::runtime_error("Failed to read JSON file: " + utf8Path);
        }

        // 确保 doc 会在函数退出时释放
        auto doc_cleanup = [&]
        { yyjson_doc_free(doc); };

        // 2. 获取根节点，应为数组
        yyjson_val *root = yyjson_doc_get_root(doc);
        if (!yyjson_is_arr(root))
        {
            doc_cleanup();
            throw std::runtime_error("JSON root is not an array");
        }

        // 3. 遍历数组
        std::vector<DanmakuItem> result;
        size_t idx, max;
        yyjson_val *item;
        yyjson_arr_foreach(root, idx, max, item)
        {
            if (!yyjson_is_obj(item))
            {
                continue; // 跳过非对象元素
            }

            DanmakuItem obj; // 临时对象，通过友元直接设置私有成员

            // 读取 fillColor (期望为整数)
            yyjson_val *val = yyjson_obj_get(item, "fillColor");
            if (val && yyjson_is_uint(val))
            {
                obj.fillColor_ = static_cast<Gdiplus::ARGB>(yyjson_get_uint(val));
            } // 若缺失或类型错误，保留默认值（0）

            // 读取 borderColor
            val = yyjson_obj_get(item, "borderColor");
            if (val && yyjson_is_uint(val))
            {
                obj.borderColor_ = static_cast<Gdiplus::ARGB>(yyjson_get_uint(val));
            }

            // 读取 text (UTF-8 字符串 -> std::wstring)
            val = yyjson_obj_get(item, "text");
            if (val && yyjson_is_str(val))
            {
                obj.text_ = Utf8ToWideString(yyjson_get_str(val));
            }

            // 读取 time (期望为有符号整数)
            val = yyjson_obj_get(item, "time");
            if (val && yyjson_is_int(val))
            {
                obj.time_ = yyjson_get_sint(val); // 或 yyjson_get_int
            }

            result.push_back(std::move(obj));
        }

        doc_cleanup();
        return result;
    }
}