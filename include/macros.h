#ifndef MACRO_H
#define MACRO_H

// 处理 ASSERT
#ifdef _MSC_VER
// 只有 msc++ 有
#define ASSERT(x) \
    if (!(x))     \
        __debugbreak();
#else
#include <signal.h>
#define ASSERT(x) \
    if (!(x))     \
        raise(SIGTRAP);
#endif

// 妙啊 用 宏 去 wrap gl 函数调用
#define GLCALL(x)     \
    gl_clear_error(); \
    x;                \
    ASSERT(gl_log_call(#x, __FILE__, __LINE__))
    // # 转为字符串 文件名 行数

#endif