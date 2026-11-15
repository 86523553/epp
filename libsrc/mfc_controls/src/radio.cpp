#include <radio.h>

BEGIN_MESSAGE_MAP(CSuRadioButton, CButton)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(BN_CLICKED, &CSuRadioButton::OnClicked)
    ON_CONTROL_REFLECT(BN_SETFOCUS, &CSuRadioButton::OnFocusIn)
    ON_CONTROL_REFLECT(BN_KILLFOCUS, &CSuRadioButton::OnFocusOut)
END_MESSAGE_MAP()

void CSuRadioButton::OnClicked() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("click", arguments.c_str());
}

void CSuRadioButton::OnFocusIn() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_in", arguments.c_str());
}

void CSuRadioButton::OnFocusOut() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_out", arguments.c_str());
}

int CSuRadioButton::set_property_json(const char* name, const char* value_json) {
    bool value = false;
    if (name != nullptr && std::strcmp(name, "checked") == 0) {
        if (!sulang::controls::json_bool(value_json, value)) {
            return sulang::controls::kInvalidValue;
        }
        SetCheck(value ? BST_CHECKED : BST_UNCHECKED);
        return sulang::controls::kSuccess;
    }
    return Base::set_property_json(name, value_json);
}

int CSuRadioButton::call_method_json(
    const char* name, const char* arguments_json, char* result, size_t result_size
) {
    if (name != nullptr && std::strcmp(name, "toggle") == 0) {
        SetCheck(GetCheck() == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED);
        return sulang::controls::write_result("null", result, result_size);
    }
    return Base::call_method_json(name, arguments_json, result, result_size);
}

