#include "custom/renderer.h"

Renderer::Renderer()
{
    std::cout << "Init aaa renderer!" << std::endl;
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader,
                    const bool gl_cull_face, const unsigned int gl_faces) const
{
    // GLCALL(glEnable(GL_DEPTH_TEST));

    if (gl_cull_face)
    {
        // GL_FRONT front face 被 cull 了 (干掉了) 只渲染后面的面
        GLCALL(glEnable(GL_CULL_FACE));
        GLCALL(glCullFace(gl_faces));
        GLCALL(glFrontFace(GL_CCW));
    }
    va.bind();
    ib.bind();
    shader.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
    if (gl_cull_face)
    {
        GLCALL(glDisable(GL_CULL_FACE));
    }
}

void Renderer::clear() const
{
    GLCALL(glClearColor(0.15f, 0.15f, 0.15f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
