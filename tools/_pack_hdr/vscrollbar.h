#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component vscrollbar
// @name 纵向滚动条
// @description 纵向滚动条（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 纵向滚动条
// @flags 窗口组件|默认跳过Tab停留
class CSuVscrollbar final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 最小位置
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MinPos);

    // @name 最大位置
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxPos);

    // @name 页改变值
    // @description 本属性指定当用户在滚动条的空隙处单击后滚动条位置的增减数值
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(PageChange);

    // @name 行改变值
    // @description 本属性指定当用户按下滚动条的上下箭头按钮后滚动条位置的增减数值
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(LineChange);

    // @name 位置
    // @description 本属性设定滚动条的现行位置
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(pos);

    // @name 允许拖动跟踪
    // @description 本属性设定当用户拖动滚动条的位置盒的过程中是否产生位置改变事件
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowTrack);

    // @name 位置被改变
    // @group 事件
    SULANG_MFC_EVENT(evt);

};