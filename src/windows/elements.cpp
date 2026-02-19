#include "pch.hpp"
#include "windows/elements.hpp"

namespace danmaku
{
    Element &Element::create()
    {
        bool needFont = false;
        switch (type)
        {
        case elementType::button:
            needFont = true;
            hwnd = CreateWindowExW(
                0,
                L"BUTTON",
                text.c_str(),
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                position.x, position.y, position.width, position.height,
                parentHwnd, (HMENU)elementID, nullptr, nullptr);
            break;
        case elementType::edit:
            needFont = true;
            hwnd = CreateWindowExW(
                0,
                L"EDIT",
                text.c_str(),
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER,
                position.x, position.y, position.width, position.height,
                parentHwnd, (HMENU)elementID, nullptr, nullptr);
            break;
        case elementType::label:
            needFont = true;
            hwnd = CreateWindowExW(
                0,
                L"STATIC",
                text.c_str(),
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                position.x, position.y, position.width, position.height,
                parentHwnd, (HMENU)elementID, nullptr, nullptr);
            break;
        default:
            debug::logOutput(L"未知元素类型：", (int)type, L"\n");
            MessageBoxW(parentHwnd, L"（0003）主窗口：尝试创建未知的元素类型", L"出错了欸", MB_ICONERROR | MB_OK);
            break;
        }
        if (needFont && elementFont)
        {
            SendMessageW(hwnd, WM_SETFONT, (WPARAM)elementFont, TRUE);
        }
        return *this;
    }

    HRESULT Element::procMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_CTLCOLORSTATIC:
        {
            // 处理color Static消息，当前版本选择采取集中处理方式，后期将修改
            [[maybe_unused]] const auto hWndCtrl = (HWND)lParam;
            const auto hdcCtrl = (HDC)wParam;

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
                    const auto *const labelInfo = (LabelExtraInfo *)extra;
                    // 设置文本颜色
                    SetTextColor(hdcCtrl, labelInfo->textColor);
                    SetBkMode(hdcCtrl, TRANSPARENT);
                    // 返回与背景颜色匹配的画刷
                    SetDCBrushColor(hdcCtrl, labelInfo->backgroundColor);
                    return (LRESULT)GetStockObject(DC_BRUSH);
                }
                default:
                    return S_FALSE;
                }
            }
        }
        case WM_COMMAND:
        {
            debug::logOutput(L"元素收到WM_COMMAND消息，ID：", LOWORD(wParam), L"，事件类型：", HIWORD(wParam), L"\n");
            // int wmId = LOWORD(wParam);
            const int wmEvent = HIWORD(wParam);

            // 按钮被点击
            if (wmEvent == BN_CLICKED)
            {
                debug::logOutput(L"元素处理按钮点击事件，ID：", LOWORD(wParam), L"\n");
                // 对于按钮元素，调用对应的事件处理函数
                if (type == elementType::button)
                {
                    // 当且仅当按钮对象拥有扩展信息时尝试调用事件处理
                    if (extra != nullptr)
                    {
                        // 调用事件处理（如果有）
                        auto *btnInfo = (ButtonExtraInfo *)extra;
                        if (btnInfo->clickProc)
                            btnInfo->clickProc();
                    }
                }
            }
            break;
        }
        default:
            return S_FALSE;
        }
        return S_OK;
    }

    Element &Element::resetFont(HFONT font)
    {
        elementFont = font;
        if (hwnd)
        {
            SendMessageW(hwnd, WM_SETFONT, (WPARAM)elementFont, TRUE);
        }
        return *this;
    }
}
