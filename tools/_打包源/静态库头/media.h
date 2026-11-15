#pragma once
#include "types.h"

// @name 播放音乐
// @description 异步播放 WAV 文件；循环播放时必须调用停止播放结束。
// @return_type 逻辑型
// @param_name file 欲播放的音乐
// @param_type file 文本型
// @param file 参数值可以为 .WAV、.MID 声音文件名称或相应格式的字节集声音数据、声音资源。
// @param_name loop 是否循环播放
// @param_type loop 逻辑型
// @param loop 可以被省略。为真表示指定音乐将被循环播放，否则仅只播放一次。如果本参数被省略，默认为仅播放一次。
bool CorePlayMusic(const std::string &file, bool loop = false);
// @name 停止播放
// @description 停止由核心支持库启动的 WAV 或 MCI 媒体播放。
void CoreStopMusic();
// @name 播放MP3
// @description 使用 Windows MCI 异步播放单个 MP3
// 文件；支持播放一次或无限循环。有限多次和文件列表因接口未提供播放队列而返回假。
// @return_type 逻辑型
// @param_name repeat_count 播放次数
// @param_type repeat_count 整数型
// @param repeat_count 为 1 时播放一次，为 -1 时无限循环；其它值当前返回假。
// @param_name file 欲播放的MP3文件名
// @param_type file 文本型
// @param file 提供参数数据时可以同时提供数组或非数组数据。
bool CorePlayMp3(std::int32_t repeat_count, const std::string &file);
// @name 播放MP3
// @description 省略播放次数时播放一次。
// @return_type 逻辑型
// @param_name file 欲播放的MP3文件名
// @param_type file 文本型
bool CorePlayMp3(const std::string &file);
// @name 同步播放MP3
// @description 从指定毫秒位置同步播放单个 MP3 文件。
// @return_type 逻辑型
// @param_name file 欲播放的MP3文件名
// @param_type file 文本型
// @param file 指定欲播放的MP3文件名。
// @param_name start_ms 起始播放位置
// @param_type start_ms 整数型
// @param start_ms 起始播放位置，单位为毫秒；负数按零处理。
bool CorePlayMp3Sync(const std::string &file, std::int32_t start_ms = 0);
// @name 暂停播放MP3
// @description 暂停当前由核心支持库打开的 MP3 播放。
void CorePauseMp3();
// @name 继续播放MP3
// @description 继续播放当前处于暂停状态的 MP3。
void CoreResumeMp3();
// @name 取MP3播放状态
// @description 返回 0 已停止、1 正在播放、-1 已暂停；无法查询时也返回 -1，以兼容原接口约定。
// @return_type 整数型
std::int32_t CoreMp3State();
