#include "custom/renderer.h"

Renderer::Renderer()
{
    std::cout << "Init a renderer!" << std::endl;
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader,
                    const bool gl_cull_face, const unsigned int gl_faces,
                    const bool enable_depth) const
{
    // GLCALL(glEnable(GL_BLEND));
    // GLCALL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
    if (enable_depth)
    {
        GLCALL(glEnable(GL_DEPTH_TEST));
    }

    if (gl_cull_face)
    {
        // GL_FRONT front face 被 cull 了 (干掉了) 只渲染后面的面
        GLCALL(glEnable(GL_CULL_FACE));
        GLCALL(glCullFace(gl_faces));
        GLCALL(glFrontFace(GL_CCW));
    }
    // bind vertex array buffer and index buffer
    va.bind();
    ib.bind();
    shader.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));

    // glDisable(GL_BLEND);

    if (gl_cull_face)
    {
        GLCALL(glDisable(GL_CULL_FACE));
    }
    if (enable_depth)
    {
        GLCALL(glDisable(GL_DEPTH_TEST));
    }
}

void Renderer::clear() const
{
    GLCALL(glClearColor(0.15f, 0.15f, 0.15f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
