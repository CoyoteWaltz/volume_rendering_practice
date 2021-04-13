#ifndef RENDERER_H
#define RENDERER_H

#include "utils.h"
#include "custom/vertex_array.h"
#include "custom/index_buffer.h"
#include "custom/shader.h"

class Renderer
{
public:
    Renderer();
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void clear() const;
};

#endif