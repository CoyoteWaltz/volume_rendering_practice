#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library for window management
// include glew before glfw because glfw needs some opengl headers in glew

#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp> // 和下面那个 transform 有啥区别??
#include <glm/vec3.hpp>                  // glm::vec3
#include <glm/vec4.hpp>                  // glm::vec4
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp>  // glm::pi
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
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

void gl_clear_error();

bool gl_log_call(const char *function, const char *file, int line);

#endif