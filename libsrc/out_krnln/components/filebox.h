#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component filebox
// @name 文件框
// @description 文件框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 文件框
// @flags 窗口组件
class CSuFilebox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 目录
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(path);

    // @name 被选择文件
    // @description 记录所有现行被选择的文件，注意不包含路径。如果有多个文件被选择，各文件名之间用分号“;”隔开
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(SelectedFile);

    // @name 允许选择多项
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowSelMutil);

    // @name 通配符
    // @description 指定允许进入文件框的文件类型。可以同时指定多个通配符，各通配符之间用分号“;”隔开
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(pattern);

    // @name 通常
    // @description 指定是否允许没有设置存档、只读、系统、隐藏文件属性的文件进入
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(normal);

    // @name 存档
    // @description 指定是否允许设置了存档文件属性的文件进入
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(arc);

    // @name 只读
    // @description 指定是否允许设置了只读文件属性的文件进入
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(ReadOnly);

    // @name 系统
    // @description 指定是否允许设置了系统文件属性的文件进入
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(sys);

    // @name 隐藏
    // @description 指定是否允许设置了隐藏文件属性的文件进入
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(hide);

    // @name 最小日期
    // @description 指定允许进入的文件的最小日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MinDate);

    // @name 最大日期
    // @description 指定允许进入的文件的最大日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxDate);

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

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

    // @name 选择文件被改变
    // @group 事件
    SULANG_MFC_EVENT(evt);

    // @name 双击选择
    // @group 事件
    // @description 当使用鼠标左键在某列表项目上双击时产生此事件
    SULANG_MFC_EVENT(evt);

};