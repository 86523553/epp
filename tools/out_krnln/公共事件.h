#pragma once

#include <component_support.h>

// 窗口组件通用事件（易语言核心库基类事件，42 个组件共享）。
// 由 开发环境/工具/gen_from_krnln.py 生成，勿手改。
// @base 窗口组件

// @name 鼠标左键被按下
// @description 用户在本组件上按下鼠标左键时触发。
// @group 事件
SULANG_MFC_EVENT(mouse_ldown);

// @name 鼠标左键被放开
// @description 用户在本组件上放开鼠标左键时触发。
// @group 事件
SULANG_MFC_EVENT(mouse_lup);

// @name 鼠标右键被按下
// @description 用户在本组件上按下鼠标右键时触发。
// @group 事件
SULANG_MFC_EVENT(mouse_rdown);

// @name 鼠标右键被放开
// @description 用户在本组件上放开鼠标右键时触发。
// @group 事件
SULANG_MFC_EVENT(mouse_rup);

// @name 鼠标位置被移动
// @description 鼠标在本组件范围内移动时触发。
// @group 事件
SULANG_MFC_EVENT(mouse_move);

// @name 滚轮被滚动
// @description 用户在本组件上滚动鼠标滚轮时触发。
// @group 事件
SULANG_MFC_EVENT(mouse_wheel);

// @name 被双击
// @description 用户双击本组件时触发。
// @group 事件
SULANG_MFC_EVENT(dblclick);

// @name 获得焦点
// @description 本组件获得输入焦点时触发。
// @group 事件
SULANG_MFC_EVENT(setfocus);

// @name 失去焦点
// @description 本组件失去输入焦点时触发。
// @group 事件
SULANG_MFC_EVENT(killfocus);

// @name 按下某键
// @description 本组件拥有焦点时按下键盘按键触发。
// @group 事件
SULANG_MFC_EVENT(keydown);

// @name 放开某键
// @description 本组件拥有焦点时放开键盘按键触发。
// @group 事件
SULANG_MFC_EVENT(keyup);

// @name 字符输入
// @description 本组件拥有焦点时输入字符触发。
// @group 事件
SULANG_MFC_EVENT(keypress);

// @name 反馈事件
// @description 由「调用反馈事件」命令向本组件发送的反馈通知。
// @group 事件
SULANG_MFC_EVENT(feedback);
