#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component combobox
// @name 组合框
// @description 组合框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 组合框
// @flags 窗口组件
class CSuCombobox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 类型
    // @description 注意: 修改本属性会丢失动态添加的项目(以原静态项目覆盖)
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:可编辑列表式／1:可编辑下拉式／2:不可编辑下拉式
    // @group 属性
    SULANG_MFC_PROPERTY(style);

    // @name 内容
    // @description 本属性指定在下拉列表框编辑部分中的文本内容。当类型为“不可编辑下拉式”时，本属性无效
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(context);

    // @name 最大文本长度
    // @description 本属性指定在下拉列表框的编辑部分所允许输入的最大文本长度，为0表示没有限制。当类型为“不可编辑下拉式”时，本属性无效
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(TextLimit);

    // @name 起始选择位置
    // @description 返回或设置所选择文本的起始点；0 为位置 1 ，1 为位置 2，如此类推。如果没有文本被选中，则指出光标位置。如果设置位置时使用值 -1，则将当前光标位置移动到文本尾部。当类型为“不可编辑下拉式”时，本属性无效
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SelStart);

    // @name 被选择字符数
    // @description 返回或设置所选择的字符数。如果设置字符数时使用值 -1，则选择所有文本。当类型为“不可编辑下拉式”时，本属性无效
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SelLength);

    // @name 被选择文本
    // @description 返回或替换当前在下拉列表框编辑部分中所选择的文本。当类型为“不可编辑下拉式”时，本属性无效
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(SelText);

    // @name 自动排序
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(sort);

    // @name 行间距
    // @description 本属性指定各列表项目行之间的间距尺寸
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(RowExtra);

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

    // @name 现行选中项
    // @description 本属性指定下拉列表框中现行被选中的列表项目的位置，位置值从 0 开始， -1 表示现行没有被选中的列表项
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(CurSel);

    // @name 列表项目
    // @description 本属性指定各列表项目
    // @type 自定义
    // @group 属性
    SULANG_MFC_PROPERTY(p_130);

    // @name 项目数值
    // @description 本属性指定与各列表项目相关联的数值，该数值在程序中可以被取出或修改
    // @type 自定义
    // @group 属性
    SULANG_MFC_PROPERTY(p_136);

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

    // @name 除去重复
    // @description 指定是否除去从数据源中取得的重复数据列数据
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(nodup);

    // @name 列表项被选择
    // @group 事件
    SULANG_MFC_EVENT(evt);

    // @name 编辑内容被改变
    // @group 事件
    SULANG_MFC_EVENT(evt);

    // @name 将弹出列表
    // @group 事件
    // @description 在将弹出组合框的列表部分之前产生此事件。在处理此事件时，可以即时填充组合框中的列表项目
    SULANG_MFC_EVENT(evt);

    // @name 列表被关闭
    // @group 事件
    // @description 在关闭被弹出的组合框列表部分后产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 双击选择
    // @group 事件
    // @description 当使用鼠标左键在某列表项目上双击时产生此事件
    SULANG_MFC_EVENT(evt);

};