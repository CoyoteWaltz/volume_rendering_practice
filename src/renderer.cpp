#include "renderer.h"
#include <iostream>

void gl_clear_error()
{
    // glGetError 返回每一个 错误码 需要进行 loop 获取到每一个错误
    while (glGetError() != GL_NO_ERROR)
    {
        // do nothing just clear
    }
}

bool gl_log_call(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[ERROR::CODE] >>> " << error
                  << " | " << function
                  << " | " << file
                  << " : " << line
                  << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer(int aa)
    : a(aa)
{
    std::cout << "aaa" << aa << std::endl;
}
