#include "custom/texture.h"
#include "vender/stb_image.h"

Texture::Texture(const std::string &file_path)
    : local_buffer(nullptr), file_path(file_path), renderer_id(0),
      width(0), height(0), bit_per_pixel(0)
{
    // flip vertically 图像 因为 OpenGL 的坐标是左下角 (0,0)
    // 图片是左上角 (0,0)
    stbi_set_flip_vertically_on_load(1);
    // 变量传 ptr
    local_buffer = stbi_load(file_path.c_str(), &width, &height, &bit_per_pixel, 4); // rgba

    GLCALL(glGenTextures(1, &renderer_id));
    GLCALL(glBindTexture(GL_TEXTURE_2D, renderer_id));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    if (local_buffer)
    {
        stbi_image_free(local_buffer);
    }
}

Texture::~Texture()
{
    GLCALL(glDeleteTextures(1, &renderer_id));
}

void Texture::bind(unsigned int slot) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot)); // texture slot 最大 32 个
    GLCALL(glBindTexture(GL_TEXTURE_2D, renderer_id));
}
void Texture::unbind()
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}