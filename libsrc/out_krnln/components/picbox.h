#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component picbox
// @name 图片框
// @description 图片框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 图片框
// @flags 窗口组件|容器|不可获焦
class CSuPicbox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 图片
    // @description 本属性指定显示在图片框上的图片
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(pic);

    // @name 显示方式
    // @description 如果设定了图片，本属性指定图片框上图片的显示方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:图片居左上／1:缩放图片／2:图片居中
    // @group 属性
    SULANG_MFC_PROPERTY(PicDrawMode);

    // @name 播放动画
    // @description 当图片为 GIF 动画格式时，通过改变此属性可控制其播放或停止。如在设计时置本属性为真，运行时动画在图片框被创建后即自动开始播放
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(PlayGif);

    // @name 数据源
    // @description 指定提供数据的数据源组件名
    // @type 属性类型(1018)
    // @group 属性
    SULANG_MFC_PROPERTY(DataSource);

    // @name 数据列
    // @description 指定数据源中的字节集或图片数据列，可以是列名或以1开始的列序号文本
    // @type 属性类型(1020)
    // @group 属性
    SULANG_MFC_PROPERTY(DataCol);

};