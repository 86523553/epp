#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component button
// @name 按钮
// @description 按钮（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 按钮
// @flags 窗口组件
class CSuButton final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 图片
    // @description 本属性指定显示在按钮上的图片
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(pic);

    // @name 类型
    // @description 当在窗口上按下Enter键时，如果没有将窗口的“回车下移输入点”属性设置为真，则等同于按下了具有“默认”类型的按钮。因此具有“默认”类型的按钮在同一窗口上应当只有一个。“通常”类型的按钮无对应的操作默认键
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:通常／1:默认
    // @group 属性
    SULANG_MFC_PROPERTY(style);

    // @name 标题
    // @description 标题只有在图片为空时才有效，如果希望某一字符成为此按钮的访问键，可以在该字符前面加上一个 & 字符
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(caption);

    // @name 横向对齐方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:左边／1:居中／2:右边
    // @group 属性
    SULANG_MFC_PROPERTY(HorzAlign);

    // @name 纵向对齐方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:顶边／1:居中／2:底边
    // @group 属性
    SULANG_MFC_PROPERTY(VertAlign);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

    // @name 被单击
    // @group 事件
    // @description 当单击按钮后即产生此事件
    SULANG_MFC_EVENT(evt);

};