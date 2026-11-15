#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component listbox
// @name 列表框
// @description 列表框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 列表框
// @flags 窗口组件
class CSuListbox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 自动排序
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(sort);

    // @name 多列
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(MultiCol);

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

    // @name 允许选择多项
    // @description 本属性指定是否允许用户同时选择列表框中的多个列表项
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowMutiSel);

    // @name 现行选中项
    // @description 本属性指定列表框中现行被选中列表项目的位置，位置值从0开始，-1表示现行没有被选中的列表项。如果“允许选择多项”属性为真，则本属性无效，其值恒为 -1
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(CurSel);

    // @name 列表项目
    // @description 本属性指定各列表项目
    // @type 自定义
    // @group 属性
    SULANG_MFC_PROPERTY(p_110);

    // @name 项目数值
    // @description 本属性指定与各列表项目相关联的数值，该数值在程序中可以被取出或修改
    // @type 自定义
    // @group 属性
    SULANG_MFC_PROPERTY(p_116);

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

    // @name 取顶端可见项目
    // @method
    // @description 返回列表框中当前最顶端可见项目的索引。0 为项目一，1 为项目二，如此类推。失败返回 -1
    SULANG_MFC_METHOD(GetTopIndex);

    // @name 置顶端可见项目
    // @method
    // @description 设置列表框中当前最顶端的可见项目，必要时将自动滚动列表框。成功返回真，失败返回假
    SULANG_MFC_METHOD(SetTopIndex);

    // @name 取项目数
    // @method
    SULANG_MFC_METHOD(GetCount);

    // @name 取项目数值
    // @method
    // @description 返回与指定项目相关联的数值。如果指定项目不存在，将返回 -1
    SULANG_MFC_METHOD(GetItemData);

    // @name 置项目数值
    // @method
    // @description 设置与指定项目相关联的数值。成功返回真，失败返回假
    SULANG_MFC_METHOD(SetItemData);

    // @name 取项目文本
    // @method
    // @description 返回指定项目的文本。如果指定项目不存在，将返回空文本
    SULANG_MFC_METHOD(GetItemText);

    // @name 置项目文本
    // @method
    // @description 设置指定项目的文本。成功返回真，失败返回假
    SULANG_MFC_METHOD(SetItemtext);

    // @name 取已选择项目数
    // @method
    // @description 返回已被选择项目的数目
    SULANG_MFC_METHOD(GetSelCount);

    // @name 取所有被选择项目
    // @method
    // @description 返回一个整数数组，內含所有当前被选择项目的位置索引。如果当前没有被选择项目，返回空数组
    SULANG_MFC_METHOD(GetSelItems);

    // @name 是否被选择
    // @method
    // @description 如果指定项目被选择，则返回真，否则返回假
    SULANG_MFC_METHOD(IsSelected);

    // @name 选择项目
    // @method
    // @description 选择或取消选择指定项目。成功返回真，失败返回假
    SULANG_MFC_METHOD(select);

    // @name 取焦点项目
    // @method
    // @description 本命令仅在多选列表框中使用，用作返回当前焦点项目的位置索引。如果在单选列表框中使用本命令，将返回当前被选择项目的位置索引
    SULANG_MFC_METHOD(GetCaretIndex);

    // @name 置焦点项目
    // @method
    // @description 本命令仅在多选列表框中使用，用作设置当前焦点项目。如果在单选列表框中使用本命令，将设置当前被选择项目。成功返回真，失败返回假
    SULANG_MFC_METHOD(SetCaretIndex);

    // @name 加入项目
    // @method
    // @description 加入指定项目到列表框的尾部，成功返回加入后该项目所处的位置，失败返回 -1
    SULANG_MFC_METHOD(AddString);

    // @name 插入项目
    // @method
    // @description 插入指定项目到列表框的指定位置处，成功返回插入后该项目所处的位置，失败返回 -1
    SULANG_MFC_METHOD(InsertString);

    // @name 删除项目
    // @method
    // @description 删除列表框指定位置处的项目。成功返回真，失败返回假
    SULANG_MFC_METHOD(DeleteString);

    // @name 清空
    // @method
    // @description 删除列表框中的所有项目
    SULANG_MFC_METHOD(clear);

    // @name 选择
    // @method
    // @description 在所有项目中寻找首部包含指定文本的项目，如找到，则选中它，并返回该项目的位置索引，否则返回 -1 。本命令仅在单选列表框中使用，如果在多选列表框中使用，将返回 -1
    SULANG_MFC_METHOD(SelItem);

    // @name 列表项被选择
    // @group 事件
    // @description 如果当前所选择的列表项或区域被改变即产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 双击选择
    // @group 事件
    // @description 当使用鼠标左键在某列表项目上双击时产生此事件
    SULANG_MFC_EVENT(evt);

};