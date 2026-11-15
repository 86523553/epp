#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component monthcalendar
// @name 月历
// @description 月历（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 月历
// @flags 窗口组件|不可获焦
class CSuMonthcalendar final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 不显示今天
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(NoToday);

    // @name 不圈注今天
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(NoTodayCircel);

    // @name 开始星期首日
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:星期一／1:星期二／2:星期三／3:星期四／4:星期五／5:星期六／6:星期日
    // @group 属性
    SULANG_MFC_PROPERTY(StartOfWeek);

    // @name 显示星期序号
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(ShowWeekNumbers);

    // @name 滚动月数
    // @description 本属性指定当按下滚动按钮改变当前显示月时，一次跳过的月的数目
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(ScrollRate);

    // @name 今天
    // @description 指定现行日期，如果不设定，则自动取系统时间中记录的现行日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(today);

    // @name 最小日期
    // @description 指定能被显示的最小日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MinDate);

    // @name 最大日期
    // @description 指定能被显示的最大日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxDate);

    // @name 允许选择多天
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowMultiSel);

    // @name 最多选择天数
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxSelCount);

    // @name 首选择日
    // @description 指定现行被选择区域的首日期。如果不允许选择多天，则本属性指定现行被选择的日期
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MinSelDay);

    // @name 尾选择日
    // @description 指定现行被选择区域的尾日期。如果不允许选择多天，则本属性无效
    // @type 日期时间型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxSelDay);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

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

    // @name 内背景颜色
    // @description 指定用来实际显示月历部分的背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(MonthBackColor);

    // @name 标题颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TitleColor);

    // @name 标题背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TitleBackColor);

    // @name 非本月颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TrailingColor);

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 选择日期被改变
    // @group 事件
    // @description 当现行被选择的日期或区域被改变后产生此事件
    SULANG_MFC_EVENT(evt);

};