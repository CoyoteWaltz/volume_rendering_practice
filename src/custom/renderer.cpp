#include "custom/renderer.h"

Renderer::Renderer()
{
    std::cout << "aaa renderer" << std::endl;
}
void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader,
                    const bool gl_cull_face, const unsigned int gl_faces) const
{
    if (gl_cull_face)
    {
        // GL_FRONT front face 被 cull 了 (干掉了) 只渲染后面的面
        GLCALL(glEnable(GL_CULL_FACE));
        GLCALL(glCullFace(gl_faces));
    }
    va.bind();
    ib.bind();
    shader.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
