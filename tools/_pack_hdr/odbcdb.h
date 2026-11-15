#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component odbcdb
// @name 外部数据库
// @description 外部数据库（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 外部数据库
// @flags 窗口组件|功能提供者
class CSuOdbcdb final : public sulang::controls::BasicControl<CWnd> {
public:

    // @name 左边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_16);

    // @name 顶边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_21);

    // @name 宽度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_26);

    // @name 高度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_31);

    // @name 标记
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(p_36);

    // @name 可视
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(p_41);

    // @name 禁止
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(p_46);

    // @name 鼠标指针
    // @type 光标
    // @group 属性
    SULANG_MFC_PROPERTY(p_51);

};