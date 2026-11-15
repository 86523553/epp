#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component hyperlinker
// @name 超级链接框
// @description 超级链接框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 超级链接框
// @flags 窗口组件|不可获焦
class CSuHyperlinker final : public sulang::controls::BasicControl<CWnd> {
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
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(caption);

    // @name 类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:电子信箱地址／1:Internet地址
    // @group 属性
    SULANG_MFC_PROPERTY(type);

    // @name 电子信箱地址
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(email);

    // @name Internet地址
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(url);

    // @name 文本颜色
    // @description 正常情况下时的文本显示颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TextColor);

    // @name 访问后的颜色
    // @description 被访问过后的文本显示颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(VisitedColor);

    // @name 热点颜色
    // @description 鼠标移动到组件上时的文本显示颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(HotColor);

    // @name 背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

};