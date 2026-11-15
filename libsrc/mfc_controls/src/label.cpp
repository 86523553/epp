#include <label.h>

BEGIN_MESSAGE_MAP(CSuLabel, CStatic)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(STN_CLICKED, &CSuLabel::OnClicked)
    ON_CONTROL_REFLECT(STN_DBLCLK, &CSuLabel::OnDoubleClicked)
END_MESSAGE_MAP()

void CSuLabel::OnClicked() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("click", arguments.c_str());
}

void CSuLabel::OnDoubleClicked() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("double_click", arguments.c_str());
}

int CSuLabel::set_property_json(const char* name, const char* value_json) {
    if (name != nullptr && std::strcmp(name, "alignment") == 0) {
        return sulang::controls::set_alignment(
            *this, value_json, SS_TYPEMASK, SS_LEFT, SS_CENTER, SS_RIGHT
        );
    }
    return Base::set_property_json(name, value_json);
}

