#pragma once

#include <mfc_component_sdk.h>

#include <atlconv.h>
#include <cstdlib>
#include <cwchar>
#include <cstring>
#include <string>
#include <vector>

namespace sulang::controls {

inline constexpr int kSuccess = 0;
inline constexpr int kUnsupported = -1;
inline constexpr int kInvalidValue = -2;

inline bool json_bool(const char* value_json, bool& value) {
    if (value_json == nullptr) {
        return false;
    }
    if (std::strcmp(value_json, "true") == 0 || std::strcmp(value_json, "1") == 0) {
        value = true;
        return true;
    }
    if (std::strcmp(value_json, "false") == 0 || std::strcmp(value_json, "0") == 0) {
        value = false;
        return true;
    }
    return false;
}

inline bool json_int(const char* value_json, int& value) {
    if (value_json == nullptr) {
        return false;
    }
    char* end = nullptr;
    const long parsed = std::strtol(value_json, &end, 10);
    if (end == value_json || *end != '\0') {
        return false;
    }
    value = static_cast<int>(parsed);
    return true;
}

inline bool json_string_at(const char*& cursor, std::string& value) {
    if (cursor == nullptr || *cursor != '"') {
        return false;
    }
    ++cursor;
    value.clear();
    while (*cursor != '\0' && *cursor != '"') {
        if (*cursor != '\\') {
            value.push_back(*cursor++);
            continue;
        }
        ++cursor;
        if (*cursor == '\0') {
            return false;
        }
        switch (*cursor) {
        case 'n': value.push_back('\n'); break;
        case 'r': value.push_back('\r'); break;
        case 't': value.push_back('\t'); break;
        case '"': value.push_back('"'); break;
        case '\\': value.push_back('\\'); break;
        default: return false;
        }
        ++cursor;
    }
    if (*cursor != '"') {
        return false;
    }
    ++cursor;
    return true;
}

inline bool json_string(const char* value_json, CString& value) {
    const char* cursor = value_json;
    std::string utf8;
    if (!json_string_at(cursor, utf8) || *cursor != '\0') {
        return false;
    }
    USES_CONVERSION;
    value = CA2W(utf8.c_str(), CP_UTF8);
    return true;
}

inline bool json_color(const char* value_json, COLORREF& value) {
    CString text;
    if (!json_string(value_json, text)) {
        return false;
    }
    text.Trim();
    if (text.GetLength() != 7 || text[0] != L'#') {
        return false;
    }
    const CString hexadecimal = text.Mid(1);
    wchar_t* end = nullptr;
    const unsigned long rgb = std::wcstoul(hexadecimal.GetString(), &end, 16);
    if (end == nullptr || *end != L'\0' || rgb > 0xFFFFFFUL) {
        return false;
    }
    value = RGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
    return true;
}

inline void skip_spaces(const char*& cursor) {
    while (*cursor == ' ' || *cursor == '\t' || *cursor == '\r' || *cursor == '\n') {
        ++cursor;
    }
}

inline bool json_string_array(const char* value_json, std::vector<CString>& values) {
    if (value_json == nullptr) {
        return false;
    }
    const char* cursor = value_json;
    skip_spaces(cursor);
    if (*cursor++ != '[') {
        return false;
    }
    values.clear();
    skip_spaces(cursor);
    while (*cursor != ']') {
        std::string utf8;
        if (!json_string_at(cursor, utf8)) {
            return false;
        }
        USES_CONVERSION;
        values.emplace_back(CA2W(utf8.c_str(), CP_UTF8));
        skip_spaces(cursor);
        if (*cursor == ',') {
            ++cursor;
            skip_spaces(cursor);
            continue;
        }
        if (*cursor != ']') {
            return false;
        }
    }
    ++cursor;
    skip_spaces(cursor);
    return *cursor == '\0';
}

inline int write_result(const char* value_json, char* result, size_t result_size) {
    if (result == nullptr || result_size == 0) {
        return kInvalidValue;
    }
    const char* value = value_json != nullptr ? value_json : "null";
    strncpy_s(result, result_size, value, _TRUNCATE);
    return kSuccess;
}

inline std::string json_escape_utf8(const CString& value) {
    USES_CONVERSION;
    const char* utf8 = CW2A(value, CP_UTF8);
    std::string result;
    for (const char* cursor = utf8; cursor != nullptr && *cursor != '\0'; ++cursor) {
        switch (*cursor) {
        case '\\': result += "\\\\"; break;
        case '"': result += "\\\""; break;
        case '\n': result += "\\n"; break;
        case '\r': result += "\\r"; break;
        case '\t': result += "\\t"; break;
        default: result.push_back(*cursor); break;
        }
    }
    return result;
}

inline std::string json_string_value(const CString& value) {
    return "\"" + json_escape_utf8(value) + "\"";
}

inline std::string text_event_arguments(CWnd& control) {
    CString value;
    control.GetWindowText(value);
    return "[" + json_string_value(value) + "]";
}

inline std::string sender_event_arguments(CWnd& control) {
    CString value;
    value.Format(L"%d", control.GetDlgCtrlID());
    return "[" + json_string_value(value) + "]";
}

inline std::string selection_event_arguments(CComboBox& control) {
    const int index = control.GetCurSel();
    CString value;
    if (index >= 0) {
        control.GetLBText(index, value);
    }
    return "[" + std::to_string(index) + "," + json_string_value(value) + "]";
}

inline std::string selection_event_arguments(CListBox& control) {
    const int index = control.GetCurSel();
    CString value;
    if (index >= 0) {
        control.GetText(index, value);
    }
    return "[" + std::to_string(index) + "," + json_string_value(value) + "]";
}

inline int set_common_property(CWnd& control, const char* name, const char* value_json) {
    if (name == nullptr) {
        return kInvalidValue;
    }
    if (std::strcmp(name, "text") == 0) {
        CString value;
        if (!json_string(value_json, value)) {
            return kInvalidValue;
        }
        control.SetWindowText(value);
        return kSuccess;
    }
    bool flag = false;
    if (std::strcmp(name, "visible") == 0) {
        if (!json_bool(value_json, flag)) {
            return kInvalidValue;
        }
        control.ShowWindow(flag ? SW_SHOW : SW_HIDE);
        return kSuccess;
    }
    if (std::strcmp(name, "enabled") == 0) {
        if (!json_bool(value_json, flag)) {
            return kInvalidValue;
        }
        control.EnableWindow(flag ? TRUE : FALSE);
        return kSuccess;
    }
    if (std::strcmp(name, "tab_stop") == 0) {
        if (!json_bool(value_json, flag)) {
            return kInvalidValue;
        }
        control.ModifyStyle(flag ? 0 : WS_TABSTOP, flag ? WS_TABSTOP : 0);
        return kSuccess;
    }
    return kUnsupported;
}

inline int call_common_method(CWnd& control, const char* name, char* result, size_t result_size) {
    if (name == nullptr) {
        return kInvalidValue;
    }
    if (std::strcmp(name, "set_focus") == 0) {
        control.SetFocus();
    } else if (std::strcmp(name, "show") == 0) {
        control.ShowWindow(SW_SHOW);
    } else if (std::strcmp(name, "hide") == 0) {
        control.ShowWindow(SW_HIDE);
    } else {
        return kUnsupported;
    }
    return write_result("null", result, result_size);
}

template<class BaseControl>
class BasicControl : public sulang::mfc::ComponentControl<BaseControl> {
public:
    ~BasicControl() {
        if (::IsWindow(this->GetSafeHwnd())) {
            this->SetFont(nullptr, FALSE);
        }
    }

    int set_property_json(const char* name, const char* value_json) override {
        if (name != nullptr && std::strcmp(name, "font_family") == 0) {
            CString family;
            if (!json_string(value_json, family)) {
                return kInvalidValue;
            }
            family.Trim();
            if (family.IsEmpty() || !apply_font(family, font_size_, font_bold_, font_italic_, font_underline_)) {
                return kInvalidValue;
            }
            font_family_ = family;
            return kSuccess;
        }
        if (name != nullptr && std::strcmp(name, "font_size") == 0) {
            int size = 0;
            if (!json_int(value_json, size) || size < 6 || size > 72) {
                return kInvalidValue;
            }
            if (!apply_font(font_family_, size, font_bold_, font_italic_, font_underline_)) {
                return kInvalidValue;
            }
            font_size_ = size;
            return kSuccess;
        }
        if (name != nullptr && std::strcmp(name, "font_bold") == 0) {
            bool value = false;
            if (!json_bool(value_json, value)
                || !apply_font(font_family_, font_size_, value, font_italic_, font_underline_)) {
                return kInvalidValue;
            }
            font_bold_ = value;
            return kSuccess;
        }
        if (name != nullptr && std::strcmp(name, "font_italic") == 0) {
            bool value = false;
            if (!json_bool(value_json, value)
                || !apply_font(font_family_, font_size_, font_bold_, value, font_underline_)) {
                return kInvalidValue;
            }
            font_italic_ = value;
            return kSuccess;
        }
        if (name != nullptr && std::strcmp(name, "font_underline") == 0) {
            bool value = false;
            if (!json_bool(value_json, value)
                || !apply_font(font_family_, font_size_, font_bold_, font_italic_, value)) {
                return kInvalidValue;
            }
            font_underline_ = value;
            return kSuccess;
        }
        if (name != nullptr && std::strcmp(name, "text_color") == 0) {
            COLORREF value = 0;
            if (!json_color(value_json, value)) {
                return kInvalidValue;
            }
            text_color_ = value;
            this->Invalidate();
            return kSuccess;
        }
        if (name != nullptr && std::strcmp(name, "background_color") == 0) {
            COLORREF value = 0;
            if (!json_color(value_json, value)) {
                return kInvalidValue;
            }
            CBrush next_brush;
            if (!next_brush.CreateSolidBrush(value)) {
                return kInvalidValue;
            }
            background_brush_.DeleteObject();
            background_brush_.Attach(static_cast<HBRUSH>(next_brush.Detach()));
            background_color_ = value;
            this->Invalidate();
            return kSuccess;
        }
        return set_common_property(*this, name, value_json);
    }

    HBRUSH CtlColor(CDC* device_context, UINT control_color) {
        (void)control_color;
        if (device_context == nullptr) {
            return nullptr;
        }
        device_context->SetTextColor(text_color_);
        device_context->SetBkColor(background_color_);
        return static_cast<HBRUSH>(background_brush_.GetSafeHandle());
    }

    int call_method_json(
        const char* name,
        const char* arguments_json,
        char* result,
        size_t result_size
    ) override {
        (void)arguments_json;
        return call_common_method(*this, name, result, result_size);
    }

private:
    bool apply_font(const CString& family, int size, bool bold, bool italic, bool underline) {
        LOGFONT description{};
        // CreatePointFontIndirect 使用十分之一磅作为输入高度。
        description.lfHeight = size * 10;
        description.lfWeight = bold ? FW_BOLD : FW_NORMAL;
        description.lfItalic = italic ? TRUE : FALSE;
        description.lfUnderline = underline ? TRUE : FALSE;
        wcsncpy_s(
            description.lfFaceName,
            _countof(description.lfFaceName),
            family.GetString(),
            _TRUNCATE
        );
        CFont next_font;
        if (!next_font.CreatePointFontIndirect(&description)) {
            return false;
        }
        CFont* previous_font = this->GetFont();
        this->SetFont(&next_font, TRUE);
        if (previous_font == &font_) {
            this->SetFont(nullptr, FALSE);
        }
        font_.DeleteObject();
        font_.Attach(static_cast<HFONT>(next_font.Detach()));
        this->SetFont(&font_, TRUE);
        return true;
    }

    CFont font_;
    CBrush background_brush_{RGB(255, 255, 255)};
    CString font_family_ = L"Microsoft YaHei UI";
    int font_size_ = 9;
    bool font_bold_ = false;
    bool font_italic_ = false;
    bool font_underline_ = false;
    COLORREF text_color_ = RGB(32, 33, 36);
    COLORREF background_color_ = RGB(255, 255, 255);
};

inline int set_alignment(CWnd& control, const char* value_json, DWORD mask, DWORD left, DWORD center, DWORD right) {
    CString value;
    if (!json_string(value_json, value)) {
        return kInvalidValue;
    }
    DWORD style = left;
    if (value == L"center") {
        style = center;
    } else if (value == L"right") {
        style = right;
    }
    control.ModifyStyle(mask, style);
    control.SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    return kSuccess;
}

}  // namespace sulang::controls
