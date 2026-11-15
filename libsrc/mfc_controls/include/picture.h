#pragma once

#include <component_support.h>

// 系统基础组件也通过支持库源码进入宿主构建，IDE 不保存控件实现。
// @component picture
// @name 图片框
// @description 用于显示图片或占位文本的图片框组件。
// @name_prefix 图片框
// @default_text "图片"
// @default_size 180 120
// @mfc_create text
// @mfc_style WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_NOTIFY
class CSuPicture final : public sulang::controls::BasicControl<CStatic> {
public:
    // @name 提示文本
    // @description 组件显示或编辑的文本内容。
    // @type 文本型
    // @default "图片"
    // @group 基本
    SULANG_MFC_PROPERTY(text);

    // @name 可见
    // @description 控制组件是否可见。
    // @type 逻辑型
    // @default true
    // @group 基本
    SULANG_MFC_PROPERTY(visible);

    // @name 启用
    // @description 控制组件是否允许交互。
    // @type 逻辑型
    // @default true
    // @group 基本
    SULANG_MFC_PROPERTY(enabled);

    // @name 字体
    // @description 设置组件文字使用的字体名称。
    // @type 字体型
    // @default "Microsoft YaHei UI"
    // @group 字体
    SULANG_MFC_PROPERTY(font_family);

    // @name 字号
    // @description 设置组件文字的磅值大小，范围为 6 到 72。
    // @type 整数型
    // @default 9
    // @group 字体
    SULANG_MFC_PROPERTY(font_size);

    // @name 字体加粗
    // @description 控制组件文字是否使用粗体。
    // @type 逻辑型
    // @default false
    // @group 字体
    SULANG_MFC_PROPERTY(font_bold);

    // @name 字体斜体
    // @description 控制组件文字是否使用斜体。
    // @type 逻辑型
    // @default false
    // @group 字体
    SULANG_MFC_PROPERTY(font_italic);

    // @name 字体下划线
    // @description 控制组件文字是否显示下划线。
    // @type 逻辑型
    // @default false
    // @group 字体
    SULANG_MFC_PROPERTY(font_underline);

    // @name 文字颜色
    // @description 设置组件文字颜色。
    // @type 颜色型
    // @default "#6b6b6b"
    // @group 字体
    SULANG_MFC_PROPERTY(text_color);

    // @name 背景颜色
    // @description 设置组件背景颜色。
    // @type 颜色型
    // @default "#ffffff"
    // @group 字体
    SULANG_MFC_PROPERTY(background_color);

    // @name 对齐方式
    // @description 设置组件内容的对齐方式。
    // @type 文本型
    // @default "center"
    // @group 字体
    SULANG_MFC_PROPERTY(alignment);

    // @name 图片路径
    // @description 设置组件显示的图片文件路径。
    // @type 文本型
    // @default ""
    // @group 基本
    SULANG_MFC_PROPERTY(image);

    // @name 获取焦点
    // @description 使组件获得输入焦点。
    // @group 方法
    SULANG_MFC_METHOD(set_focus);

    // @name 显示
    // @description 显示组件。
    // @group 方法
    SULANG_MFC_METHOD(show);

    // @name 隐藏
    // @description 隐藏组件。
    // @group 方法
    SULANG_MFC_METHOD(hide);

    // @name 清除图片
    // @description 清除当前显示的图片。
    // @group 方法
    SULANG_MFC_METHOD(clear_image);

    // @name 被单击
    // @description 用户单击组件时触发。
    // @param_name sender 发送者
    // @param_type sender 文本型
    // @param sender 触发事件的组件名称
    // @group 事件
    SULANG_MFC_EVENT(click, sender);

    // @name 被双击
    // @description 用户双击组件时触发。
    // @param_name sender 发送者
    // @param_type sender 文本型
    // @param sender 触发事件的组件名称
    // @group 事件
    SULANG_MFC_EVENT(double_click, sender);

    using Base = sulang::controls::BasicControl<CStatic>;
    ~CSuPicture() override {
        if (image_ != nullptr) {
            ::DeleteObject(image_);
        }
    }
    int set_property_json(const char* name, const char* value_json) override;
    int call_method_json(const char* name, const char* arguments_json, char* result, size_t result_size) override;

protected:
    afx_msg void OnClicked();
    afx_msg void OnDoubleClicked();
    DECLARE_MESSAGE_MAP()

private:
    HBITMAP image_ = nullptr;
};
