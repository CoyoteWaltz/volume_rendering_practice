#ifndef LIBS_H
#define LIBS_H

#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library for window management
// include glew before glfw because glfw needs some opengl headers in glew

// #include <glm/vec3.hpp>                  // glm::vec3
// #include <glm/vec4.hpp>                  // glm::vec4
// #include <glm/mat4x4.hpp>                // glm::mat4
// #include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
// #include <glm/ext/matrix_clip_space.hpp> // glm::perspective
// #include <glm/ext/scalar_constants.hpp>  // glm::pi
// #include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "vender/stb_image.h"

// custom class 注意这些 header 本身不要 include libs.h 会有依赖混乱
#include "custom/vertex_buffer.h"
#include "custom/index_buffer.h"
#include "custom/vertex_array.h"
#include "custom/shader.h"
#include "custom/vertex_buffer_layout.h"
#include "custom/renderer.h"
#include "custom/texture.h"

#endif