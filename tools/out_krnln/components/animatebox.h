#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component animatebox
// @name 影像框
// @description 影像框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 影像框
// @flags 窗口组件|不可获焦
class CSuAnimatebox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 文件名
    // @description 本属性指定欲播放影像文件的名称
    // @type 文本型
    // @ptype 文件名
    // @choices 请输入影像文件的名称：／影像文件 (*.avi)／*.avi／avi／0
    // @group 属性
    SULANG_MFC_PROPERTY(file);

    // @name 居中播放
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(center);

    // @name 透明背景
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(transparent);

    // @name 播放
    // @description 如果指定了有效的影像文件，通过改变此属性可控制其播放或停止。如在设计时置本属性为真，运行时影像文件在影像框被创建后即自动开始播放
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(play);

    // @name 播放次数
    // @description 指定影像的播放次数。如为 -1，则无限次地循环播放
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(PlayCount);

};