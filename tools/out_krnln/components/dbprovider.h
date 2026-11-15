#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component dbprovider
// @name 数据库提供者
// @description 数据库提供者（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 数据库提供者
// @flags 窗口组件|功能提供者
class CSuDbprovider final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 数据库文件名
    // @type 文本型
    // @ptype 文件名
    // @choices 请输入数据库文件的名称：／数据库文件 (*.edb)／*.edb／edb／0
    // @group 属性
    SULANG_MFC_PROPERTY(dbfile);

    // @name 字节集字段处理
    // @description 本属性指定对字节集类型字段的处理方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:跳过／1:视为图片数据／2:视为字节集数据
    // @group 属性
    SULANG_MFC_PROPERTY(BinMode);

    // @name 数据库密码
    // @description 本属性提供访问数据库时所需要的密码
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(Password);

};