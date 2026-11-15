#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component odbcprovider
// @name 外部数据提供者
// @description 外部数据提供者（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 外部数据提供者
// @flags 窗口组件|功能提供者
class CSuOdbcprovider final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 连接文本
    // @description 本属性指定用作打开数据库的ODBC连接文本
    // @type 属性类型(1021)
    // @group 属性
    SULANG_MFC_PROPERTY(connect);

    // @name 查询SQL
    // @description 本属性用作指定数据库中的数据表名或者用作查询记录集的SELECT类SQL语句
    // @type 属性类型(1022)
    // @group 属性
    SULANG_MFC_PROPERTY(sql);

};