#include "debug.h"

#include <stdexcept>
#include <windows.h>

void CoreDebugBreak()
{
#ifndef NDEBUG
    if (IsDebuggerPresent())
        DebugBreak();
#endif
}
void CoreAssert(bool condition)
{
    if (!condition)
        throw std::runtime_error("核心支持库检查失败");
}
bool CoreIsDebugBuild()
{
#ifdef NDEBUG
    return false;
#else
    return true;
#endif
}
