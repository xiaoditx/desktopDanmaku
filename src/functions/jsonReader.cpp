#include "yyjson/yyjson.h"
#include "pch.hpp"
#include <cstdint>
#include <stdexcept>
#include "danmaku/dmkitem.hpp"
#include "debug.hpp"

/*
json文件示例：(ARGB)

[
    {
        "fillColor": "#ff9BFF13",
        "borderColor": "#ff000000",
        "text": "Ciallo",
        "time": 1234567890
    },
    {
        "fillColor": "#4855B7FF",
        "borderColor": "#ffB700FF",
        "text": "World",
        "time": 9876543210
    }
]

*/

namespace danmaku
{
    // yyjson文档智能指针
    struct yyjson_doc_deleter
    {
        void operator()(yyjson_doc *doc) const noexcept
        {
            if (doc)
                yyjson_doc_free(doc);
        }
    };
    using yyjson_doc_ptr = std::unique_ptr<yyjson_doc, yyjson_doc_deleter>;

    // 辅助函数：将 UTF-8 字符串转换为宽字符串（std::wstring）
    static std::wstring Utf8ToWideString(const char *utf8)
    {
        if (!utf8)
            return {};
        int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
        if (len <= 0)
            return {};
        std::wstring result(len - 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, result.data(), len);
        return result;
    }

    // 辅助函数：将宽字符串（std::wstring）转换为 UTF-8 字符串
    static std::string WideStringToUtf8(const std::wstring &wstr)
    {
        if (wstr.empty())
            return {};
        int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (len <= 0)
            throw std::runtime_error("字符串转换失败：无法转换宽字符串为UTF-8");
        std::string result(len - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result.data(), len, nullptr, nullptr);
        return result;
    }

    /**
     * @brief 从 JSON 文件中读取弹幕数组
     * @param filename JSON 文件路径（宽字符串）
     * @return 包含 DanmakuItem 对象的动态数组
     */
    std::vector<DanmakuItem> ReadDanmakuArrayFromJsonFile(const std::wstring &filename)
    {
        // 将宽字符串路径转换为 UTF-8
        std::string utf8Path = WideStringToUtf8(filename);

        if (!FileExists(filename.c_str()))
        {
            throw std::runtime_error("JSON文件不存在: " + utf8Path);
        }

        // 使用 yyjson 读取文件
        yyjson_doc_ptr doc(yyjson_read_file(utf8Path.c_str(), 0, nullptr, nullptr));
        if (!doc)
        {
            throw std::runtime_error("读取JSON文件失败: " + utf8Path);
        }

        // 获取根节点，应为数组
        yyjson_val *root = yyjson_doc_get_root(doc.get());
        if (!yyjson_is_arr(root))
        {
            throw std::runtime_error("JSON文件根节点不是数组: " + utf8Path);
        }

        // 遍历数组
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

            // 读取 fillColor
            yyjson_val *val = yyjson_obj_get(item, "fillColor");
            if (val)
            {
                obj.fillColor_ = hexStringToArgb(Utf8ToWideString(yyjson_get_str(val)));
            } // 若缺失，保留默认值

            // 读取 borderColor
            val = yyjson_obj_get(item, "borderColor");
            if (val)
            {
                obj.borderColor_ = hexStringToArgb(Utf8ToWideString(yyjson_get_str(val)));
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

        return result;
    }
}