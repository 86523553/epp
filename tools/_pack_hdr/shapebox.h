#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component shapebox
// @name 外形框
// @description 外形框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 外形框
// @flags 窗口组件|容器|不可获焦
class CSuShapebox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 外形
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:矩形／1:正方形／2:椭圆／3:圆／4:圆角矩形／5:圆角正方形／6:横向线／7:纵向线
    // @group 属性
    SULANG_MFC_PROPERTY(shape);

    // @name 线条效果
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:通常／1:凹入／2:凸出
    // @group 属性
    SULANG_MFC_PROPERTY(effect);

    // @name 线型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无／1:直线／2:划线／3:点线／4:点划线／5:双点划线
    // @group 属性
    SULANG_MFC_PROPERTY(LineStyle);

    // @name 线宽
    // @description 注意：仅在线条效果为“通常”且线型为“直线”时才有效
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(LineWidth);

    // @name 线条颜色
    // @description 注意：仅在线条效果为“通常”时才有效
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(LineColor);

    // @name 填充颜色
    // @description 注意：仅在外形不为线条时才有效
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(FillColor);

};