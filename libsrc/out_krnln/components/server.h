#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component server
// @name 服务器
// @description 服务器（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 服务器
// @flags 窗口组件|功能提供者
class CSuServer final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 取回数据
    // @method
    // @description 取回所接收到的数据。本命令必须在“数据到达”事件的处理子程序中使用
    SULANG_MFC_METHOD(recv);

    // @name 取回客户
    // @method
    // @description 当接收到“客户进入”、“客户离开”或“数据到达”事件时，在该事件的处理子程序中可调用本方法取回对应的客户地址（IP地址 + 端口）
    SULANG_MFC_METHOD(GetClient);

    // @name 发送数据
    // @method
    // @description 向指定已经连接进来的客户发送数据。成功返回真，失败返回假
    SULANG_MFC_METHOD(send);

    // @name 断开客户
    // @method
    // @description 断开与指定客户之间的连接
    SULANG_MFC_METHOD(CloseClient);

    // @name 数据到达
    // @group 事件
    // @description 当有数据到达后，会产生本事件。在本事件的处理子程序中调用“取回数据”方法即可取回本次所收到的数据
    SULANG_MFC_EVENT(evt);

    // @name 客户进入
    // @group 事件
    // @description 当有新客户连接入本服务器组件后，会产生本事件。在本事件的处理子程序中调用“取回客户”方法即可取回此新客户的地址（IP地址 + 端口）
    SULANG_MFC_EVENT(evt);

    // @name 客户离开
    // @group 事件
    // @description 当有已连接客户断开与本服务器组件的连接后，会产生本事件。在本事件的处理子程序中调用“取回客户”方法即可取回此客户的地址（IP地址 + 端口）
    SULANG_MFC_EVENT(evt);

};