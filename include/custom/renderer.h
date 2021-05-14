#ifndef RENDERER_H
#define RENDERER_H

#include "utils.h"
#include "custom/vertex_array.h"
#include "custom/index_buffer.h"
#include "custom/shader.h"

class Renderer
{
public:
    Renderer(unsigned int width, unsigned int height, unsigned int texture2d_id);
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader, const bool gl_cull_face, const unsigned int gl_faces) const;
    void clear() const;
    void bind_fbo() const;
    void unbind_fbo() const;

    inline unsigned int get_tex_id() const { return tex_id; }

private:
    unsigned int framebuffer_id;
    unsigned int depthbuffer_id;
    unsigned int tex_id;
};

#endif