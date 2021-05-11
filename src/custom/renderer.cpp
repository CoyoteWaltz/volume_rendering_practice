#include "custom/renderer.h"

Renderer::Renderer()
{
    std::cout << "Init aaa renderer!" << std::endl;
    // GLuint depthBuffer;
    // glGenRenderbuffers(1, &depthBuffer);
    // glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);

    // // attach the texture and the depth buffer to the framebuffer
    // glGenFramebuffers(1, &g_frameBuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, g_frameBuffer);
    // // 给 frame buffer 绑定一个指定 level 的 mipmap 注意 0 是原始图
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texObj, 0);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    // // checkFramebufferStatus();
    // glEnable(GL_DEPTH_TEST);
    // GLCALL(glEnable(GL_DEPTH_TEST));
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
    if (gl_cull_face)
    {

        GLCALL(glDisable(GL_CULL_FACE));
    }
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
