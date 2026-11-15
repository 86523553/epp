#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component picbtn
// @name 图形按钮
// @description 图形按钮（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 图形按钮
// @flags 窗口组件|不可获焦
class CSuPicbtn final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:按钮／1:选择框
    // @group 属性
    SULANG_MFC_PROPERTY(type);

    // @name 选中
    // @description 当类型为选择框时，本属性为当前选中状态
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(Checked);

    // @name 正常图片
    // @description 本属性指定在正常情况下所显示图片(不支持ICO或CUR图片数据)
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(npic);

    // @name 点燃图片
    // @description 当类型为按钮时，本属性指定当鼠标移动到按钮上时所显示图片(不支持ICO或CUR图片数据)
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(apic);

    // @name 按下图片
    // @description 本属性指定当选择框被选中或者按钮被按下后所显示图片(不支持ICO或CUR图片数据)
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(ppic);

    // @name 禁止图片
    // @description 本属性指定当按钮被禁止后所显示图片(不支持ICO或CUR图片数据)
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(dpic);

    // @name 透明颜色
    // @description 指定所有图片中透明部分的颜色。如果为“#默认色”常量值，则表示无透明色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(tclr);

    // @name 被单击
    // @group 事件
    // @description 当单击按钮后即产生此事件
    SULANG_MFC_EVENT(evt);

};