#ifndef RENDERER_H
#define RENDERER_H

#include "utils.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

class Renderer
{
public:
    Renderer();
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
};

#endif