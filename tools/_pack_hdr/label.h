#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component label
// @name 标签
// @description 标签（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 标签
// @flags 窗口组件|不可获焦
class CSuLabel final : public sulang::controls::BasicControl<CWnd> {
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
    // @description 如果希望某一字符成为本标签后可停留焦点组件的访问键，可以在该字符前面加上一个 & 字符
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(caption);

    // @name 效果
    // @description 如果将本属性值设置为"透明"(在设计模式下无效果,不支持在运行时修改),则标签的背景将为透明,此时"背景颜色"属性将不起作用.
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:通常／1:凹入／2:凸出／3:阴影／4:透明
    // @group 属性
    SULANG_MFC_PROPERTY(effect);

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式／6:渐变镜框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 渐变边框宽度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_78);

    // @name 渐变边框颜色1
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(p_84);

    // @name 渐变边框颜色2
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(p_90);

    // @name 渐变边框颜色3
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(p_96);

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

    // @name 底图
    // @description 本属性指定显示在标签背景上的图片
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(BackPic);

    // @name 底图方式
    // @description 如果设定了底图，本属性指定标签背景上底图的显示方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:图片居左上／1:图片平铺／2:图片居中
    // @group 属性
    SULANG_MFC_PROPERTY(BackPicMode);

    // @name 渐变背景方式
    // @description 本属性只有在未设定底图的前提下才有效
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无渐变背景／1:从上到下／2:从左到右／3:从左上到右下／4:从右上到左下／5:从下到上／6:从右到左／7:从右下到左上／8:从左下到右上
    // @group 属性
    SULANG_MFC_PROPERTY(p_136);

    // @name 渐变背景颜色1
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(p_142);

    // @name 渐变背景颜色2
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(p_148);

    // @name 渐变背景颜色3
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(p_154);

    // @name 横向对齐方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:左对齐／1:居中／2:右对齐
    // @group 属性
    SULANG_MFC_PROPERTY(HorzAlignMode);

    // @name 是否自动折行
    // @description 本属性设定当文本内容超出右界后是否自动转到下一行显示
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(p_167);

    // @name 纵向对齐方式
    // @description 如果选用自动折行则本属性无效
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:顶对齐／1:居中／2:底对齐
    // @group 属性
    SULANG_MFC_PROPERTY(VertAlignMode);

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

    // @name 反馈事件
    // @group 事件
    // @description 本事件用作与外部DLL交互或者其它场合，事件子程序处理完毕后可以根据需要决定是否返回值（注：信息值为32885）
    SULANG_MFC_EVENT(feedback);

};