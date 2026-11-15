#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component tab
// @name 选择夹
// @description 选择夹（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 选择夹
// @flags 窗口组件|容器|Tab组件|不可获焦
class CSuTab final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 表头方向
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:上／1:下／2:左／3:右
    // @group 属性
    SULANG_MFC_PROPERTY(HeaderWay);

    // @name 允许多行表头
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowMultiLines);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

    // @name 子夹管理
    // @type 自定义
    // @group 属性
    SULANG_MFC_PROPERTY(p_74);

    // @name 现行子夹
    // @description 本属性指定现行被选中子夹的索引，索引值从 0 开始
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(CurTab);

    // @name 隐藏自身
    // @description 本属性可以用来动态切换数个同一位置处子组件群的显示。此时选择夹本身不会被显示，但用户可以通过改变其“现行子夹”属性来显示指定子夹内的所有子组件。注意在设计时为方便用户定位子组件，表头方向始终为下。另外本属性不支持运行时修改
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(HideMe);

    // @name 是否填充背景
    // @description “隐藏自身”属性为真时如将本属性设置为假，可能会引起选择夹内一些子组件的刷新问题，此时将此属性设置为真并指定一个合适的背景颜色即可解决此问题
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(FillBack);

    // @name 背景颜色
    // @description 用作填充被隐藏选择夹的背景的颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 被单击
    // @group 事件
    // @description 当在选择夹上单击后产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 将改变子夹
    // @group 事件
    // @description 当现行被选择子夹即将被改变时产生此事件。返回假中止此改变，返回真或不返回值则允许此改变
    SULANG_MFC_EVENT(evt);

    // @name 子夹被改变
    // @group 事件
    // @description 当现行被选择子夹已经被改变时产生此事件
    SULANG_MFC_EVENT(evt);

};