#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component groupbox
// @name 分组框
// @description 分组框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 分组框
// @flags 窗口组件|容器|不可获焦
class CSuGroupbox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 标题
    // @description 如果希望某一字符成为本分组框中第一个可停留焦点组件的访问键，可以在该字符前面加上一个 & 字符
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(caption);

    // @name 对齐方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:左边／1:居中／2:右边
    // @group 属性
    SULANG_MFC_PROPERTY(AlignMode);

    // @name 文本颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TextColor);

    // @name 背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

};