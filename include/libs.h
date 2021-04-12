#ifndef LIBS_H
#define LIBS_H

#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library for window management
// include glew before glfw because glfw needs some opengl headers in glew
#include <iostream> //for cout
#include <string>
#include <sstream> // string stream
#include <fstream>
#include <vector>

// #include <glm/vec3.hpp>                  // glm::vec3
// #include <glm/vec4.hpp>                  // glm::vec4
// #include <glm/mat4x4.hpp>                // glm::mat4
// #include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
// #include <glm/ext/matrix_clip_space.hpp> // glm::perspective
// #include <glm/ext/scalar_constants.hpp>  // glm::pi
// #include <glm/gtc/type_ptr.hpp>

// custom class 注意这些 header 本身不要 include libs.h 会有依赖混乱
#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_buffer_layout.h"
#include "vertex_array.h"
#include "shader.h"

#endif