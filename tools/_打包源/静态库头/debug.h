#pragma once
#include "types.h"

// @name 暂停
// @description 调试构建中触发调试断点；发布构建中不执行操作。
void CoreDebugBreak();
// @name 检查
// @description 条件为假时抛出 runtime_error，保证发布构建也不会静默忽略失败。
// @param_name condition 被校验的条件
// @param_type condition 逻辑型
// @param condition 如果此条件不能成立，即参数值为假，易程序的执行将被暂停且警示。
void CoreAssert(bool condition);
// @name 是否为调试版
// @description 根据 NDEBUG 编译宏返回当前支持库是否为调试构建。
// @return_type 逻辑型
bool CoreIsDebugBuild();
