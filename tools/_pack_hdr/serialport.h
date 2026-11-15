#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component serialport
// @name 端口
// @description 端口（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 端口
// @flags 窗口组件|功能提供者
class CSuSerialport final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 端口号
    // @description 端口号从 1 开始
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(port);

    // @name 波特率
    // @description 指定通信端口所使用的波特率，比较常用的波特率有：110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200, 128000, 256000
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(band);

    // @name 数据位数
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(databits);

    // @name 停止位数
    // @description 可以为以下值之一：1: 1位;  2: 1.5位;  3: 2位
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(stopbits);

    // @name 奇偶校验
    // @description 指定通信时对数据是否进行奇偶校验
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(parity);

    // @name 事件字符
    // @description 指定用作触发事件的字符。只使用文本的首字符，为空表示不支持事件字符
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(EventChar);

    // @name 等待时间
    // @description 指定在进行端口读写操作时最长等待时间。单位毫秒
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(WaitTimeout);

    // @name 自动启动
    // @description 在对端口进行操作之前必须首先启动，本属性指定当组件被载入时是否自动启动
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AutoStart);

    // @name 奇偶校验方案
    // @description 指定通信时所使用的具体奇偶校验位方案
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无／1:奇校验／2:偶校验／3:标志校验／4:空白校验
    // @group 属性
    SULANG_MFC_PROPERTY(ParityScheme);

    // @name 流控制方案
    // @description 指定通信时所使用的流控制方案
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无／1:CtsRts／2:CtsDtr／3:DsrRts／4:DsrDtr／5:XonXoff
    // @group 属性
    SULANG_MFC_PROPERTY(FlowCtrl);

    // @name Rts流控制
    // @description 指定通信时是否启用Rts流控制
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:默认／1:禁止／2:启用
    // @group 属性
    SULANG_MFC_PROPERTY(RtsFlowCtrl);

    // @name Dtr流控制
    // @description 指定通信时是否启用Dtr流控制
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:默认／1:禁止／2:启用
    // @group 属性
    SULANG_MFC_PROPERTY(DtrFlowCtrl);

    // @name 收到信号
    // @group 事件
    // @description 当通信端口的信号状态发生改变时，会产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 数据到达
    // @group 事件
    // @description 每当通信端口接收到一个数据字节，就会产生本事件
    SULANG_MFC_EVENT(evt);

};