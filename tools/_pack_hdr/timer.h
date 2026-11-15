#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component timer
// @name 时钟
// @description 时钟（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 时钟
// @flags 窗口组件|功能提供者
class CSuTimer final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 时钟周期
    // @description 指定时钟事件产生的周期，单位为毫秒。如果设定为0，则无时钟事件产生
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(elapse);

    // @name 周期事件
    // @group 事件
    // @description 每当经过了指定的时钟周期，将产生此事件
    SULANG_MFC_EVENT(evt);

};