#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component grid
// @name 表格
// @description 表格（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 表格
// @flags 窗口组件
class CSuGrid final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 数据源
    // @description 指定提供数据的数据源组件名
    // @type 属性类型(1018)
    // @group 属性
    SULANG_MFC_PROPERTY(DataSource);

    // @name 表格线颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(LineColor);

    // @name 背景颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 缩放比
    // @description 指定显示表格时所使用的缩放百分比，值在 20 到 1000 之间
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(Scale);

    // @name 允许选择块
    // @description 指定是否允许操作者选择表格单元格区域
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(EnableSelection);

    // @name 显示标尺
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(ShowRuler);

    // @name 显示空表格线
    // @description 指定是否用虚线显示空表格线
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(ShowHide);

    // @name 禁止调整行高
    // @description 指定是否禁止操作者调整表格行高
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(DisableResizeRow);

    // @name 禁止调整列宽
    // @description 指定是否禁止操作者调整表格列宽
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(DisableResizeCol);

    // @name 允许粘贴扩展
    // @description 指定当粘贴表格数据时，如果现行表格尺寸无法容纳，是否允许自动扩展
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AutoExtend);

    // @name 置光标
    // @method
    SULANG_MFC_METHOD(SetCaret);

    // @name 选择
    // @method
    SULANG_MFC_METHOD(Select);

    // @name 全部选择
    // @method
    SULANG_MFC_METHOD(SelectAll);

    // @name 取光标行号
    // @method
    SULANG_MFC_METHOD(CaretRow);

    // @name 取光标列号
    // @method
    SULANG_MFC_METHOD(CaretCol);

    // @name 取选择行数
    // @method
    // @description 被选择的行区域起始于光标所在行
    SULANG_MFC_METHOD(SelectRows);

    // @name 取选择列数
    // @method
    // @description 被选择的列区域起始于光标所在列
    SULANG_MFC_METHOD(SelectCols);

    // @name 等宽缩放
    // @method
    // @description 将表格内容缩放到与表格窗口等宽显示
    SULANG_MFC_METHOD(InWin);

    // @name 全部复制
    // @method
    // @description 复制所有表格数据到剪贴板
    SULANG_MFC_METHOD(CopyAll);

    // @name 复制
    // @method
    // @description 复制指定区域表格数据到剪贴板
    SULANG_MFC_METHOD(Copy);

    // @name 粘贴到光标处
    // @method
    // @description 粘贴剪贴板中的表格数据到当前光标处
    SULANG_MFC_METHOD(PasteToCaret);

    // @name 粘贴
    // @method
    // @description 粘贴剪贴板中的表格数据到指定位置
    SULANG_MFC_METHOD(Paste);

    // @name 打印
    // @method
    // @description 打印表格数据到打印机。注意欲设置打印份数等打印设置信息请到与表格相连接的数据源中设置。成功返回空文本，失败(不包含操作者中断打印)返回非空错误文本
    SULANG_MFC_METHOD(Print);

    // @name 打印预览
    // @method
    SULANG_MFC_METHOD(PrintPreview);

    // @name 光标位置改变
    // @group 事件
    // @description 当操作者改变了光标位置后会产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 选择行列数改变
    // @group 事件
    // @description 当操作者更改了当前被选择区域的行列数时会产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 内容被改变
    // @group 事件
    // @description 当操作者修改了单元格内容时即产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 行高被改变
    // @group 事件
    // @description 当操作者调整了表格行行高时即产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 列宽被改变
    // @group 事件
    // @description 当操作者调整了表格列列宽时即产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 尺寸被扩展
    // @group 事件
    // @description 当操作者粘贴来自剪贴板的表格单元数据时，如果为了容纳该数据自动扩展了表格尺寸，即产生此事件
    SULANG_MFC_EVENT(evt);

};