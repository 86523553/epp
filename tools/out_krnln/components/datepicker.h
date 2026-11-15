#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component datepicker
// @name 日期框
// @description 日期框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 日期框
// @flags 窗口组件
class CSuDatepicker final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 允许编辑
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowEdit);

    // @name 附件类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:下拉月历／1:调节器
    // @group 属性
    SULANG_MFC_PROPERTY(SubType);

    // @name 今天
    // @description 指定或读取现行日期，如果不设定，则自动取现行日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(today);

    // @name 最小日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MinDate);

    // @name 最大日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxDate);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

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

    // @name 选择日期被改变
    // @group 事件
    // @description 当现行被选择的日期被改变后产生此事件
    SULANG_MFC_EVENT(evt);

};