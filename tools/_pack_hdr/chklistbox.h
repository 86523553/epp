#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component chklistbox
// @name 选择列表框
// @description 选择列表框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 选择列表框
// @flags 窗口组件
class CSuChklistbox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 单选
    // @description 本属性指定是否只允许选择一项列表项
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(OnlyPickOne);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

    // @name 现行选中项
    // @description 本属性指定列表框中现行被选中的列表项目的位置，位置值从0开始，-1表示现行没有被选中的列表项
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(CurSel);

    // @name 列表项目
    // @description 本属性指定各列表项目
    // @type 自定义
    // @group 属性
    SULANG_MFC_PROPERTY(p_98);

    // @name 项目数值
    // @description 本属性指定与各列表项目相关联的数值，该数值在程序中可以被取出或修改
    // @type 自定义
    // @group 属性
    SULANG_MFC_PROPERTY(p_104);

    // @name 列表项被选择
    // @group 事件
    // @description 如果当前所选择的列表项被改变即产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 选中状态被改变
    // @group 事件
    SULANG_MFC_EVENT(evt);

    // @name 双击选择
    // @group 事件
    // @description 当使用鼠标左键在某列表项目上双击时产生此事件
    SULANG_MFC_EVENT(evt);

};