#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component checkbox
// @name 选择框
// @description 选择框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 选择框
// @flags 窗口组件
class CSuCheckbox final : public sulang::controls::BasicControl<CWnd> {
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
    // @description 本属性指定显示在选择框上的图片
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(pic);

    // @name 按钮形式
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(PushLike);

    // @name 平面
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(flat);

    // @name 标题
    // @description 标题只有在图片为空时才有效，如果希望某一字符成为此选择框的访问键，可以在该字符前面加上一个 & 字符
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(caption);

    // @name 标题居左
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(LeftCaption);

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

    // @name 选中
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(Checked);

    // @name 数据源
    // @description 指定提供数据的数据源组件名
    // @type 属性类型(1018)
    // @group 属性
    SULANG_MFC_PROPERTY(DataSource);

    // @name 数据列
    // @description 指定数据源中的数据列，可以是列名或以1开始的列序号文本
    // @type 属性类型(1020)
    // @group 属性
    SULANG_MFC_PROPERTY(DataCol);

    // @name 被单击
    // @group 事件
    // @description 当单击选择框后即产生此事件
    SULANG_MFC_EVENT(evt);

};