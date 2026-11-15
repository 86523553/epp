#include <combo.h>

BEGIN_MESSAGE_MAP(CSuComboBox, CComboBox)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(CBN_SELCHANGE, &CSuComboBox::OnSelectionChanged)
    ON_CONTROL_REFLECT(CBN_SETFOCUS, &CSuComboBox::OnFocusIn)
    ON_CONTROL_REFLECT(CBN_KILLFOCUS, &CSuComboBox::OnFocusOut)
END_MESSAGE_MAP()

void CSuComboBox::OnSelectionChanged() {
    const std::string arguments = sulang::controls::selection_event_arguments(*this);
    emit_event("selection_changed", arguments.c_str());
}

void CSuComboBox::OnFocusIn() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_in", arguments.c_str());
}

void CSuComboBox::OnFocusOut() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_out", arguments.c_str());
}

int CSuComboBox::set_property_json(const char* name, const char* value_json) {
    if (name != nullptr && std::strcmp(name, "items") == 0) {
        std::vector<CString> values;
        if (!sulang::controls::json_string_array(value_json, values)) {
            return sulang::controls::kInvalidValue;
        }
        ResetContent();
        for (const CString& value : values) {
            AddString(value);
        }
        return sulang::controls::kSuccess;
    }
    if (name != nullptr && std::strcmp(name, "current_index") == 0) {
        int value = -1;
        if (!sulang::controls::json_int(value_json, value)) {
            return sulang::controls::kInvalidValue;
        }
        SetCurSel(value);
        return sulang::controls::kSuccess;
    }
    return Base::set_property_json(name, value_json);
}

int CSuComboBox::call_method_json(
    const char* name, const char* arguments_json, char* result, size_t result_size
) {
    if (name != nullptr && std::strcmp(name, "clear") == 0) {
        ResetContent();
        return sulang::controls::write_result("null", result, result_size);
    }
    return Base::call_method_json(name, arguments_json, result, result_size);
}

