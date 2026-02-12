#include "windows/elements.hpp"

namespace danmaku
{
    UINT_PTR element::currentElementID = 0;
    element &element::create()
    {
        bool needFont = false;
        switch (type)
        {
        case elementType::button:
            needFont = true;
            hwnd = CreateWindow(L"BUTTON", text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                position.x, position.y, position.width, position.height, parentHwnd, nullptr, GetModuleHandle(nullptr), nullptr);
            break;
        case elementType::edit:
            needFont = true;
            hwnd = CreateWindow(L"EDIT", text.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT,
                                position.x, position.y, position.width, position.height, parentHwnd, nullptr, GetModuleHandle(nullptr), nullptr);
            break;
        case elementType::label:
            needFont = true;
            elementID = currentElementID;
            hwnd = CreateWindow(L"STATIC", text.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT,
                                position.x, position.y, position.width, position.height, parentHwnd, (HMENU)currentElementID++, GetModuleHandle(nullptr), nullptr);
            break;
        default:
            MessageBox(parentHwnd, L"（0003）主窗口：尝试创建未知的元素类型", L"出错了欸", MB_ICONERROR | MB_OK);
            break;
        }
        if (needFont && elementFont)
        {
            SendMessage(hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(elementFont), TRUE);
        }
        return *this;
    }
}
