#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component udp
// @name 数据报
// @description 数据报（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 数据报
// @flags 窗口组件|功能提供者
class CSuUdp final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 端口
    // @description 指定监听数据到达的端口号，可以是大于 0 小于 32767 的任何自定数值
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(port);

    // @name 发送数据
    // @method
    // @description 发送数据到指定主机上的指定端口。成功返回真，失败返回假
    SULANG_MFC_METHOD(send);

    // @name 取回数据
    // @method
    // @description 取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用
    SULANG_MFC_METHOD(recv);

    // @name 数据到达
    // @group 事件
    // @description 当有数据到达后，会产生本事件。在本事件的处理子程序中调用“取回数据”方法即可取回本次所收到的数据
    SULANG_MFC_EVENT(evt);

};