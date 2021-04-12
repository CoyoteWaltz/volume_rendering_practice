#include "renderer.h"

Renderer::Renderer()
{
    std::cout << "aaa renderer" << std::endl;
}
void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader)
{
    va.bind();
    ib.bind();
    shader.bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}
