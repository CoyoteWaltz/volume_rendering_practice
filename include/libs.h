#ifndef LIBS_H
#define LIBS_H

#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

// custom class 注意这些 header 本身不要 include libs.h 会有依赖混乱
#include "custom/vertex_buffer.h"
#include "custom/index_buffer.h"
#include "custom/vertex_array.h"
#include "custom/shader.h"
#include "custom/vertex_buffer_layout.h"
#include "custom/renderer.h"
#include "custom/texture.h"

#endif