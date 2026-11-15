#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component client
// @name 客户
// @description 客户（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 客户
// @flags 窗口组件|功能提供者
class CSuClient final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 数据到达
    // @group 事件
    // @description 当服务器端将数据发送过来后，会产生本事件。在本事件的处理子程序中调用“取回数据”方法即可取回本次所收到的数据
    SULANG_MFC_EVENT(evt);

    // @name 连接断开
    // @group 事件
    // @description 当连接被服务器端断开后，会产生本事件
    SULANG_MFC_EVENT(evt);

};