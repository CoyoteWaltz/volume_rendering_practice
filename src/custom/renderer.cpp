#include "custom/renderer.h"

Renderer::Renderer(unsigned int width, unsigned int height, unsigned int texture2d_id)
{
    return;
    GLCALL(glGenTextures(1, &tex_id));
    GLCALL(glBindTexture(GL_TEXTURE_2D, tex_id));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    // GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    // GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));

    // internal type GL_RGBA16F 和我们在 glsl 中如何存的 有关
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
                        GL_RGBA, GL_FLOAT, NULL));

    std::cout << "Init aaa renderer!" << std::endl;
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    GLCALL(glActiveTexture(GL_TEXTURE0 + 1)); // texture slot 最大 32 个
    GLCALL(glBindTexture(GL_TEXTURE_2D, tex_id));

    // attach the texture and the depth buffer to the framebuffer
    GLCALL(glGenFramebuffers(1, &framebuffer_id));
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id));
    // 给 frame buffer 绑定一个指定 level 的 mipmap 注意 0 是原始图
    GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0));
    GLCALL(glDrawBuffer(GL_COLOR_ATTACHMENT0));

    // 渲染缓冲区
    GLCALL(glGenRenderbuffers(1, &depthbuffer_id));
    GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer_id));
    GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));

    std::cout << "----ssss " << tex_id << std::endl;
    // 绑定一个 depth buffer
    GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer_id));

    GLCALL(unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        switch (status)
        {
        case GL_FRAMEBUFFER_COMPLETE:
            std::cout << "Framebuffer complete." << std::endl;
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "[ERROR Framebuffer] incomplete: Attachment is NOT complete." << std::endl;
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "[ERROR Framebuffer] incomplete: No image is attached to FBO." << std::endl;
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            std::cout << "[ERROR Framebuffer] incomplete: Attached images have different dimensions." << std::endl;
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            std::cout << "[ERROR Framebuffer] incomplete: Color attached images have different internal formats." << std::endl;
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "[ERROR Framebuffer] incomplete: Draw buffer." << std::endl;
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "[ERROR Framebuffer] incomplete: Read buffer." << std::endl;
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "[ERROR Unsupported] by FBO implementation." << std::endl;
            break;

        default:
            std::cout << "[ERROR] Unknow error." << std::endl;
            break;
        }
        exit(EXIT_FAILURE);
    }

    unbind_fbo();
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader,
                    const bool gl_cull_face, const unsigned int gl_faces) const
{
    GLCALL(glEnable(GL_DEPTH_TEST));

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
    GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 0.f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::bind_fbo() const
{
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id)); // 这里绑定 framebuffer 把第一个 draw 的数据画到 fbo 的 texture
}

void Renderer::unbind_fbo() const
{
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // 这里绑定 framebuffer 把第一个 draw 的数据画到 fbo 的 texture
}
