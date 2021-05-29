#include "custom/frame_buffer.h"

FrameBuffer::FrameBuffer(const unsigned int width, const unsigned int height)
{
    GLCALL(glGenTextures(1, &color_buffer_id));
    GLCALL(glBindTexture(GL_TEXTURE_2D, color_buffer_id));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));

    // internal type GL_RGBA16F 和我们在 glsl 中如何存的 有关
    // 不过这里的 size 放大了 2 倍 说实话我也不知道为什么 tmd 就 * 2 没问题了
    int scale = 2;
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width * scale, height * scale, 0,
                        GL_RGBA, GL_FLOAT, NULL));

    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    // GLCALL(glActiveTexture(GL_TEXTURE0 + 1)); // texture slot 最大 32 个
    // GLCALL(glBindTexture(GL_TEXTURE_2D, color_buffer_id));

    // attach the texture and the depth buffer to the framebuffer
    GLCALL(glGenFramebuffers(1, &renderer_id));
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, renderer_id));
    // 给 frame buffer 绑定一个指定 level 的 mipmap 注意 0 是原始图
    GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer_id, 0));
    GLCALL(glDrawBuffer(GL_COLOR_ATTACHMENT0));

    // 渲染缓冲区
    // 绑定一个 depth buffer
    GLCALL(glGenRenderbuffers(1, &depth_buffer_id));
    GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id));
    GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height));
    GLCALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id));

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

    unbind();
}
FrameBuffer::~FrameBuffer()
{
    GLCALL(glDeleteFramebuffers(1, &renderer_id));
    GLCALL(glDeleteFramebuffers(1, &depth_buffer_id));
}

void FrameBuffer::bind() const
{
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, renderer_id)); // 这里绑定 framebuffer 把第一个 draw 的数据画到 fbo 的 texture
    // GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderer_id)); // 这里绑定 framebuffer 把第一个 draw 的数据画到 fbo 的 texture
}
void FrameBuffer::unbind() const
{
    GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::bind_color_buffer_to_texture(const int slot) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot)); // texture slot 最大 32 个
    GLCALL(glBindTexture(GL_TEXTURE_2D, color_buffer_id));
}

void FrameBuffer::unbind_color_buffer_to_texture() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}