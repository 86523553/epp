#include <edit.h>

BEGIN_MESSAGE_MAP(CSuEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(EN_CHANGE, &CSuEdit::OnTextChanged)
    ON_CONTROL_REFLECT(EN_SETFOCUS, &CSuEdit::OnFocusIn)
    ON_CONTROL_REFLECT(EN_KILLFOCUS, &CSuEdit::OnFocusOut)
END_MESSAGE_MAP()

void CSuEdit::OnTextChanged() {
    const std::string arguments = sulang::controls::text_event_arguments(*this);
    emit_event("text_changed", arguments.c_str());
}

void CSuEdit::OnFocusIn() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_in", arguments.c_str());
}

void CSuEdit::OnFocusOut() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_out", arguments.c_str());
}

int CSuEdit::set_property_json(const char* name, const char* value_json) {
    bool value = false;
    if (name != nullptr && std::strcmp(name, "read_only") == 0) {
        if (!sulang::controls::json_bool(value_json, value)) {
            return sulang::controls::kInvalidValue;
        }
        SetReadOnly(value ? TRUE : FALSE);
        return sulang::controls::kSuccess;
    }
    if (name != nullptr && std::strcmp(name, "password") == 0) {
        if (!sulang::controls::json_bool(value_json, value)) {
            return sulang::controls::kInvalidValue;
        }
        SetPasswordChar(value ? L'*' : 0);
        return sulang::controls::kSuccess;
    }
    if (name != nullptr && std::strcmp(name, "alignment") == 0) {
        return sulang::controls::set_alignment(
            *this, value_json, ES_LEFT | ES_CENTER | ES_RIGHT, ES_LEFT, ES_CENTER, ES_RIGHT
        );
    }
    return Base::set_property_json(name, value_json);
}

int CSuEdit::call_method_json(
    const char* name, const char* arguments_json, char* result, size_t result_size
) {
    if (name != nullptr && std::strcmp(name, "clear") == 0) {
        SetWindowText(L"");
        return sulang::controls::write_result("null", result, result_size);
    }
    if (name != nullptr && std::strcmp(name, "select_all") == 0) {
        SetSel(0, -1);
        return sulang::controls::write_result("null", result, result_size);
    }
    return Base::call_method_json(name, arguments_json, result, result_size);
}

