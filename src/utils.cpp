#include "utils.h"

void gl_clear_error()
{
    // glGetError 返回每一个 错误码 需要进行 loop 获取到每一个错误
    while (glGetError() != GL_NO_ERROR)
    {
        // do nothing just clear
        // 但问题是 如果 glfwTerminate call 了 析构的时候没有 OpenGL context
        // 这里会一直报错 一个 死循环
        // 用 new 去构造 或者 给一个新的 scope
    }
}

// 关于 error code 
// (1280).toString(16)  -> '500'
// find hex define in glew.h
bool gl_log_call(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[ERROR::CODE] >>> " << error
                  << " | " << function
                  << " in " << file
                  << ":" << line
                  << std::endl;
        return false;
    }
    return true;
}