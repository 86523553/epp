#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component datasrc
// @name 数据源
// @description 数据源（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 数据源
// @flags 窗口组件
class CSuDatasrc final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 数据提供者
    // @description 指定本数据源所基于的数据提供者组件名，所有数据存取操作都将通过数据提供者实际完成
    // @type 属性类型(1019)
    // @group 属性
    SULANG_MFC_PROPERTY(DataProvider);

    // @name 只读
    // @description 指定是否允许操作者修改数据源内的数据，注意本属性并不限制在程序中直接修改数据
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(ReadOnly);

    // @name 允许添加
    // @description 指定是否允许操作者添加记录到数据源内，注意本属性并不限制在程序中直接添加记录
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowAppend);

    // @name 允许删除
    // @description 指定是否允许操作者删除数据源内的记录，注意本属性并不限制在程序中直接删除记录
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowDelete);

    // @name 当前记录改变
    // @group 事件
    // @description 当操作者按下移动记录按钮移动了当前记录指针后，会产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 添加记录
    // @group 事件
    // @description 当操作者按下添加记录按钮添加了新记录后，会产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 删除记录
    // @group 事件
    // @description 当操作者按下删除记录按钮删除了当前记录后，会产生本事件
    SULANG_MFC_EVENT(evt);

};