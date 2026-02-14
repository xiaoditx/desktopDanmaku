#include "windows/elements.hpp"

namespace danmaku
{
    element &element::create()
    {
        bool needFont = false;
        switch (type)
        {
        case elementType::button:
            needFont = true;
            hwnd = CreateWindow(L"BUTTON", text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                position.x, position.y, position.width, position.height, parentHwnd, (HMENU)elementID, GetModuleHandle(nullptr), nullptr);
            break;
        case elementType::edit:
            needFont = true;
            hwnd = CreateWindow(L"EDIT", text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT,
                                position.x, position.y, position.width, position.height, parentHwnd, (HMENU)elementID, GetModuleHandle(nullptr), nullptr);
            break;
        case elementType::label:
            needFont = true;
            debug::logOutput(L"标签创建：", elementID, "\n");
            hwnd = CreateWindow(L"STATIC", text.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT,
                                position.x, position.y, position.width, position.height, parentHwnd, (HMENU)elementID, GetModuleHandle(nullptr), nullptr);
            break;
        default:
            debug::logOutput(L"未知元素类型：", static_cast<int>(type), L"\n");
            MessageBox(parentHwnd, L"（0003）主窗口：尝试创建未知的元素类型", L"出错了欸", MB_ICONERROR | MB_OK);
            break;
        }
        if (needFont && elementFont)
        {
            SendMessage(hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(elementFont), TRUE);
        }
        return *this;
    }

    HRESULT element::procMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CTLCOLORSTATIC:
        {
            debug::logOutput(L"color static 消息，对象ID：", elementID, L"，", text);
            // 处理color Static消息，当前版本选择采取集中处理方式，后期将修改
            HWND hWndCtrl = (HWND)lParam;
            HDC hdcCtrl = (HDC)wParam;

            if (extra == nullptr)
            {
                return S_FALSE;
            }
            else
            {
                switch (this->type)
                {
                case elementType::label:
                {
                    // 设置文本颜色
                    SetTextColor(hdcCtrl, reinterpret_cast<labelExtraInfo *>(extra)->textColor);
                    // 设置背景模式为不透明，这样背景颜色才会生效
                    SetBkMode(hdcCtrl, OPAQUE);
                    // 设置背景颜色
                    SetBkColor(hdcCtrl, reinterpret_cast<labelExtraInfo *>(extra)->backgroundColor);
                    // 返回与背景颜色匹配的画刷
                    return (LRESULT)CreateSolidBrush(reinterpret_cast<labelExtraInfo *>(extra)->backgroundColor);
                }
                default:
                    return S_FALSE;
                }
            }
        }
        default:
            return S_FALSE;
        }
    }

    element &element::resetFont(HFONT font)
    {
        elementFont = font;
        if (hwnd)
        {
            SendMessage(hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(elementFont), TRUE);
        }
        return *this;
    }
}
