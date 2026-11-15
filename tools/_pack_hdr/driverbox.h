#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component driverbox
// @name 驱动器框
// @description 驱动器框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 驱动器框
// @flags 窗口组件
class CSuDriverbox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 驱动器
    // @description 指定当前驱动器符，如果不指定，默认为当前驱动器。驱动器符为大写字母 A - Z
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(drive);

    // @name 类型
    // @description 指定可用的驱动器类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:全部驱动器／1:可抽取式驱动器／2:固定驱动器／3:光盘驱动器／4:网络驱动器／5:内存虚拟驱动器
    // @group 属性
    SULANG_MFC_PROPERTY(type);

    // @name 文本颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TextColor);

    // @name 背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

    // @name 驱动器被改变
    // @group 事件
    SULANG_MFC_EVENT(evt);

};