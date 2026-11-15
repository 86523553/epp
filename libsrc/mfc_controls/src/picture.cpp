#include <picture.h>

BEGIN_MESSAGE_MAP(CSuPicture, CStatic)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(STN_CLICKED, &CSuPicture::OnClicked)
    ON_CONTROL_REFLECT(STN_DBLCLK, &CSuPicture::OnDoubleClicked)
END_MESSAGE_MAP()

void CSuPicture::OnClicked() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("click", arguments.c_str());
}

void CSuPicture::OnDoubleClicked() {
    const std::string arguments = sulang::controls::sender_event_arguments(*this);
    emit_event("double_click", arguments.c_str());
}

int CSuPicture::set_property_json(const char* name, const char* value_json) {
    if (name != nullptr && std::strcmp(name, "alignment") == 0) {
        return sulang::controls::set_alignment(
            *this, value_json, SS_TYPEMASK, SS_LEFT, SS_CENTER, SS_RIGHT
        );
    }
    if (name != nullptr && std::strcmp(name, "image") == 0) {
        CString path;
        if (!sulang::controls::json_string(value_json, path)) {
            return sulang::controls::kInvalidValue;
        }
        if (image_ != nullptr) {
            ::DeleteObject(image_);
            image_ = nullptr;
        }
        if (!path.IsEmpty()) {
            image_ = static_cast<HBITMAP>(::LoadImageW(
                nullptr, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION
            ));
            if (image_ == nullptr) {
                return sulang::controls::kInvalidValue;
            }
        }
        SetBitmap(image_);
        return sulang::controls::kSuccess;
    }
    return Base::set_property_json(name, value_json);
}

int CSuPicture::call_method_json(
    const char* name, const char* arguments_json, char* result, size_t result_size
) {
    if (name != nullptr && std::strcmp(name, "clear_image") == 0) {
        SetBitmap(nullptr);
        if (image_ != nullptr) {
            ::DeleteObject(image_);
            image_ = nullptr;
        }
        return sulang::controls::write_result("null", result, result_size);
    }
    return Base::call_method_json(name, arguments_json, result, result_size);
}

