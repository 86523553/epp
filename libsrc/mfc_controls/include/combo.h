#pragma once

#include <component_support.h>

// 系统基础组件也通过支持库源码进入宿主构建，IDE 不保存控件实现。
// @component combo
// @name 组合框
// @description 用于从下拉选项中选择内容的组合框组件。
// @name_prefix 组合框
// @default_text ""
// @default_size 160 28
// @mfc_create plain
// @mfc_style WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL
class CSuComboBox final : public sulang::controls::BasicControl<CComboBox> {
public:
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

    // @name Tab 停留
    // @description 控制按 Tab 键时是否停留到此组件。
    // @type 逻辑型
    // @default true
    // @group 基本
    SULANG_MFC_PROPERTY(tab_stop);

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
    // @default "#202124"
    // @group 字体
    SULANG_MFC_PROPERTY(text_color);

    // @name 背景颜色
    // @description 设置组件背景颜色。
    // @type 颜色型
    // @default "#ffffff"
    // @group 字体
    SULANG_MFC_PROPERTY(background_color);

    // @name 选项
    // @description 设置组件显示的项目集合。
    // @type 文本型数组
    // @default []
    // @group 基本
    SULANG_MFC_PROPERTY(items);

    // @name 当前索引
    // @description 设置当前选中项目的索引。
    // @type 整数型
    // @default -1
    // @group 基本
    SULANG_MFC_PROPERTY(current_index);

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

    // @name 清空
    // @description 清空组件当前内容。
    // @group 方法
    SULANG_MFC_METHOD(clear);

    // @name 选择改变
    // @description 组件当前选择发生变化时触发。
    // @param_name index 索引
    // @param_type index 整数型
    // @param index
    // @param_name text 文本
    // @param_type text 文本型
    // @param text
    // @group 事件
    SULANG_MFC_EVENT(selection_changed, index, text);

    // @name 获得焦点
    // @description 组件获得输入焦点时触发。
    // @param_name sender 发送者
    // @param_type sender 文本型
    // @param sender 获得焦点的组件名称
    // @group 事件
    SULANG_MFC_EVENT(focus_in, sender);

    // @name 失去焦点
    // @description 组件失去输入焦点时触发。
    // @param_name sender 发送者
    // @param_type sender 文本型
    // @param sender 失去焦点的组件名称
    // @group 事件
    SULANG_MFC_EVENT(focus_out, sender);

    using Base = sulang::controls::BasicControl<CComboBox>;
    int set_property_json(const char* name, const char* value_json) override;
    int call_method_json(const char* name, const char* arguments_json, char* result, size_t result_size) override;

protected:
    afx_msg void OnSelectionChanged();
    afx_msg void OnFocusIn();
    afx_msg void OnFocusOut();
    DECLARE_MESSAGE_MAP()
};
