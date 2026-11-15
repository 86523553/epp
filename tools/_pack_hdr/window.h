#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component window
// @name 窗口
// @description 窗口（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 窗口
// @flags 窗口组件|容器
class CSuWindow final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 标题
    // @description 本属性指定窗口的标题文本
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(caption);

    // @name 边框
    // @description 本属性指定窗口边框的类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:普通可调边框／2:普通固定边框／3:窄标题可调边框／4:窄标题固定边框／5:镜框式可调边框／6:镜框式固定边框
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 底色
    // @description 本属性指定窗口背景的颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 底图
    // @description 本属性指定显示在窗口背景上的图片
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(BackPic);

    // @name 底图方式
    // @description 如果设定了底图，本属性指定窗口背景上底图的显示方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:图片居左上／1:图片平铺／2:图片居中／3:缩放图片
    // @group 属性
    SULANG_MFC_PROPERTY(BackPicMode);

    // @name 背景音乐
    // @description 本属性指定当窗口调入时自动开始播放的音乐
    // @type 字节集
    // @ptype 音乐文件
    // @group 属性
    SULANG_MFC_PROPERTY(BackMusic);

    // @name 播放次数
    // @description 本属性指定背景音乐的播放次数
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:循环播放／1:仅播放一次／2:不播放
    // @group 属性
    SULANG_MFC_PROPERTY(PlayCount);

    // @name 控制按钮
    // @description 本属性设置是否显示窗口上的所有控制按钮及窗口控制菜单
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(CtrlBtn);

    // @name 最大化按钮
    // @description 本属性设置是否显示窗口右上角的最大化或还原按钮
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxBtn);

    // @name 最小化按钮
    // @description 本属性设置是否显示窗口右上角的最小化按钮
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(MinBtn);

    // @name 位置
    // @description 本属性设置窗口的位置及状态。注意如果欲设置为“最大化”，“边框”属性必须不为固定类边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:通常／1:居中／2:最小化／3:最大化
    // @group 属性
    SULANG_MFC_PROPERTY(pos);

    // @name 可否移动
    // @description 本属性设置窗口位置能否被移动
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(CanMove);

    // @name 图标
    // @description 本属性设置窗口左上角所显示的图标
    // @type 字节集
    // @ptype 图标文件
    // @group 属性
    SULANG_MFC_PROPERTY(icon);

    // @name 回车下移焦点
    // @description 本属性如果设置为真，运行时当用户在本窗口上按下回车键后，将自动将当前焦点转移到下一个组件。本属性如果设置为假，当用户在本窗口上按下回车键后，将等同于按下具有“默认”属性的按钮
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(EnterToNext);

    // @name Esc键关闭
    // @description 本属性设置运行时用户是否可以用 Esc 键关闭本窗口
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(EscClose);

    // @name F1键打开帮助
    // @description 本属性设置运行时用户是否可以用 F1 键打开有关本窗口的帮助信息
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(F1OpenHelp);

    // @name 帮助文件名
    // @description 指定欲打开帮助文件的名称，如果没有指定帮助文件，将使用默认帮助文件名称，即本易程序的文件名称（不含其后缀）＋“.hlp”后缀
    // @type 文本型
    // @ptype 文件名
    // @choices 请输入帮助文件的名称：／帮助文件 (*.hlp)／*.hlp／hlp／0
    // @group 属性
    SULANG_MFC_PROPERTY(HelpFileName);

    // @name 帮助标志值
    // @description 用于在帮助文件中指定相应的帮助标志数值。本属性如果为零，则显示帮助主题
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(HelpContext);

    // @name 在任务条中显示
    // @description 设置本窗口是否出现在 Windows 系统的任务条中
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(ShowInTaskBar);

    // @name 随意移动
    // @description 设置当用户在窗口上任何位置按下鼠标左键后是否进入窗口整体移动模式。注意如果本属性设置为真，则在本窗口及其中所有不能接收输入焦点的子组件上均不能再接收到“鼠标左键被按下”和“鼠标左键被放开”事件
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(HitMove);

    // @name 外形
    // @description 设置窗口的外形
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:矩形／1:椭圆／2:圆角矩形／3:环／4:正三角／5:倒三角／6:左三角／7:右三角／8:平行四边形／9:五边形／10:六边形／11:梯形／12:菱形／13:五角星／14:十字星／15:闪电形／16:爆炸形1／17:爆炸形2／18:燕尾／19:折角矩形／20:左箭头／21:右箭头／22:上箭头／23:下箭头／24:左右箭头／25:上下箭头／26:十字箭头／27:丁字箭头／28:燕尾箭头／29:五边形箭头
    // @group 属性
    SULANG_MFC_PROPERTY(shape);

    // @name 总在最前
    // @description 设置本窗口永远位于其它窗口的前面
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(Topmost);

    // @name 保持标题条激活
    // @description 如果希望本窗口在非激活状态下其标题条仍然与激活状态时相同，请设置本属性为真
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(KeepTitleBarActive);

    // @name 窗口类名
    // @description 指定本窗口在创建时所使用的窗口类名。本属性仅设计时设置有效
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(ClassName);

    // @name 创建完毕
    // @group 事件
    // @description 当窗口及其中的所有组件均被创建后在显示之前产生此事件，用户可以在响应此事件期间做一些初始化工作。本事件为该窗口的第一个事件
    SULANG_MFC_EVENT(evt);

    // @name 可否被关闭
    // @group 事件
    // @description 在窗口及计算机被关闭之前产生此事件，用于询问该关闭操作是否允许被继续。如果返回假则不允许关闭，返回真或不返回值允许关闭
    SULANG_MFC_EVENT(evt);

    // @name 将被销毁
    // @group 事件
    // @description 当窗口将被销毁前产生此事件。窗口被销毁后所有对此窗口及其内部组件的修改都将被作废
    SULANG_MFC_EVENT(evt);

    // @name 位置被改变
    // @group 事件
    // @description 当窗口的位置被改变时产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 尺寸被改变
    // @group 事件
    // @description 当窗口的尺寸被改变后产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 被激活
    // @group 事件
    // @description 当窗口转换为激活状态时产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 被取消激活
    // @group 事件
    // @description 当窗口转换为非激活状态时产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 空闲
    // @group 事件
    // @description 当系统正处于空闲状态时产生此事件，用户可以在响应此事件时做一些后台处理工作。如果事件处理子程序返回假或者不返回值，在此次空闲期间系统将不再产生空闲事件（可以降低 CPU 占用率）。如果返回真，系统将继续产生空闲事件
    SULANG_MFC_EVENT(evt);

    // @name 首次激活
    // @group 事件
    // @description 当窗口创建完毕后被首次激活显示时产生此事件，在此事件及此事件后用户可以开始在窗口内的画板组件上写出内容
    SULANG_MFC_EVENT(evt);

    // @name 托盘事件
    // @group 事件
    // @description 当用户用鼠标单击或双击本窗口对象“置托盘图标”方法所设置的图标后即产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 被显示
    // @group 事件
    // @description 当窗口被显示时产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 被隐藏
    // @group 事件
    // @description 当窗口被隐藏时产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 窗口可否被关闭
    // @group 事件
    // @description 在窗口被关闭之前产生此事件，用于询问窗口可否被关闭。如果返回假则不允许窗口被关闭，返回真或不返回值允许关闭
    SULANG_MFC_EVENT(evt);

};