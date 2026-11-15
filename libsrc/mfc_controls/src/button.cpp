#include <button.h>

BEGIN_MESSAGE_MAP(CSuButton, CButton)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(BN_CLICKED, &CSuButton::OnClicked)
    ON_CONTROL_REFLECT(BN_DOUBLECLICKED, &CSuButton::OnDoubleClicked)
    ON_CONTROL_REFLECT(BN_SETFOCUS, &CSuButton::OnFocusIn)
    ON_CONTROL_REFLECT(BN_KILLFOCUS, &CSuButton::OnFocusOut)
END_MESSAGE_MAP()

void CSuButton::OnClicked() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("click", arguments.c_str());
}

void CSuButton::OnDoubleClicked() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("double_click", arguments.c_str());
}

void CSuButton::OnFocusIn() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_in", arguments.c_str());
}

void CSuButton::OnFocusOut() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("focus_out", arguments.c_str());
}

int CSuButton::set_property_json(const char* name, const char* value_json) {
    if (name != nullptr && std::strcmp(name, "alignment") == 0) {
        return sulang::controls::set_alignment(
            *this, value_json, BS_LEFT | BS_CENTER | BS_RIGHT, BS_LEFT, BS_CENTER, BS_RIGHT
        );
    }
    return Base::set_property_json(name, value_json);
}

int CSuButton::call_method_json(
    const char* name, const char* arguments_json, char* result, size_t result_size
) {
    if (name != nullptr && std::strcmp(name, "click") == 0) {
        SendMessage(BM_CLICK);
        return sulang::controls::write_result("null", result, result_size);
    }
    return Base::call_method_json(name, arguments_json, result, result_size);
}

