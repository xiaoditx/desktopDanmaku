#ifndef DANMAKU_FONT_HPP
#define DANMAKU_FONT_HPP
#include <windows.h>
namespace danmaku
{
    // Font 类封装了 Windows API 中的字体对象，提供了创建和管理字体的功能
    class Font
    {
    private:
        HFONT hFont = nullptr; // 存储字体句柄，以便后续操作使用
    public:
        Font() = default;
        // 创建字体对象，参数包括字体名称、大小、加粗、斜体、下划线和删除线等属性
        Font(const wchar_t *fontName, int fontSize,
             bool bold = false, bool italic = false,
             bool underline = false, bool strikeout = false)
        {
            create(fontName, fontSize, bold, italic, underline, strikeout);
        }

        HFONT create(const wchar_t *fontName, int fontSize,
                     bool bold = false, bool italic = false,
                     bool underline = false, bool strikeout = false)
        {
            if (hFont)
                DeleteObject(hFont);
            return hFont = CreateFontW(
                       fontSize, 0, 0, 0,
                       bold ? FW_BOLD : FW_NORMAL,
                       italic ? TRUE : FALSE,
                       underline ? TRUE : FALSE,
                       strikeout ? TRUE : FALSE,
                       DEFAULT_CHARSET,
                       OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_DONTCARE,
                       fontName);
        }
        operator HFONT() const { return hFont; }  // 提供隐式转换为 HFONT 类型的操作符，方便在需要字体句柄的地方直接使用 Font 对象
        HFONT getHandle() const { return hFont; } // 提供获取字体句柄的成员函数，返回当前字体对象的句柄
        ~Font()
        {
            if (hFont)
                DeleteObject(hFont); // 删除字体对象，释放资源
        }
    };
}
#endif // DANMAKU_FONT_HPP