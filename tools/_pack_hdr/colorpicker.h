#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component colorpicker
// @name 颜色选择器
// @description 颜色选择器（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 颜色选择器
// @flags 窗口组件
class CSuColorpicker final : public sulang::controls::BasicControl<CWnd> {
public:

    // @name 左边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(left);

    // @name 顶边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(top);

    // @name 宽度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(width);

    // @name 高度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(height);

    // @name 标记
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(tag);

    // @name 可视
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(visible);

    // @name 禁止
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(disable);

    // @name 鼠标指针
    // @type 字节集
    // @ptype 光标
    // @group 属性
    SULANG_MFC_PROPERTY(MousePointer);

    // @name 颜色
    // @description 颜色数值为红绿蓝颜色值或“#透明”常量值
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(color);

    // @name 允许透明
    // @description 指定是否允许透明色
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowTransparent);

    // @name 颜色被改变
    // @group 事件
    // @description 跳转到指定的邮件或 Internet 地址
    SULANG_MFC_EVENT(evt);

};