#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component spin
// @name 调节器
// @description 调节器（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 调节器
// @flags 窗口组件|不可获焦
class CSuSpin final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 方向
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:横向／1:纵向
    // @group 属性
    SULANG_MFC_PROPERTY(orient);

    // @name 热点跟踪
    // @description 指定当鼠标移动到组件上时是否显示热点状态
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(HotTrack);

    // @name 调节钮被按下
    // @group 事件
    SULANG_MFC_EVENT(evt);

};