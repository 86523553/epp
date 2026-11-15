#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component sliderbar
// @name 滑块条
// @description 滑块条（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 滑块条
// @flags 窗口组件
class CSuSliderbar final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 方向
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:横向／1:纵向
    // @group 属性
    SULANG_MFC_PROPERTY(orient);

    // @name 刻度类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无／1:上/左／2:下/右／3:双向
    // @group 属性
    SULANG_MFC_PROPERTY(TickStyle);

    // @name 单位刻度值
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(TickFreq);

    // @name 允许选择
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowSel);

    // @name 首选择位置
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SelStart);

    // @name 选择长度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SelLength);

    // @name 页改变值
    // @description 本属性指定当用户在空隙处单击后滑块条位置的增减数值
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(PageChange);

    // @name 行改变值
    // @description 本属性指定当用户拖动滑块时滑块条位置的每次增减数值
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(LineChange);

    // @name 最小位置
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MinPos);

    // @name 最大位置
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxPos);

    // @name 位置
    // @description 本属性设定滑块条的现行位置
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(pos);

    // @name 位置被改变
    // @group 事件
    SULANG_MFC_EVENT(evt);

};