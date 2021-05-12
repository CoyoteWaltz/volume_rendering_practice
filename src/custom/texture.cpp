#include "custom/texture.h"
#include "vender/stb_image.h"
#include <fstream>

Texture1D::Texture1D(const std::string &file_path, const bool empty)
    : Texture(GL_TEXTURE_1D), local_buffer(nullptr), file_path(file_path),
      length(0)
{
    std::ifstream file(file_path, std::ifstream::in);
    if (!file)
    {
        std::cerr << "[LOAD::Error::TEXTURE1D opening file: " << file_path << std::endl;
        exit(EXIT_FAILURE);
    }

    const int MAX_CNT = 10000;
    // local_buffer = (unsigned char *)calloc(MAX_CNT, sizeof(unsigned char));
    local_buffer = new unsigned char[MAX_CNT];
    file.read(reinterpret_cast<char *>(local_buffer), MAX_CNT);
    // for (size_t i = 0; i < MAX_CNT; i++)
    // {
    //     /* code */
    //     int data = int(local_buffer[i]);
    //     if (data != 0)
    //     {
    //         std::cout << "local_buffer " << data << std::endl;
    //     }
    // }

    if (file.eof())
    {
        size_t bytecnt = file.gcount();
        *(local_buffer + bytecnt) = '\0';
        std::cout << "bytecnt " << bytecnt << std::endl;
    }
    else if (file.fail())
    {
        std::cout << file_path << "read failed " << std::endl;
    }
    else
    {
        std::cout << file_path << "is too large" << std::endl;
    }
    glGenTextures(1, &renderer_id);
    glBindTexture(type, renderer_id);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //
    glTexImage1D(type, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
    delete[] local_buffer;
    local_buffer = nullptr;
}

Texture1D::~Texture1D()
{
    GLCALL(glDeleteTextures(1, &renderer_id));
}

//////// Texture2D
Texture2D::Texture2D(const std::string &file_path, const bool empty)
    : Texture(GL_TEXTURE_2D), local_buffer(nullptr), file_path(file_path),
      width(0), height(0)
{
    // flip vertically 图像 因为 OpenGL 的坐标是左下角 (0,0)
    // 图片是左上角 (0,0)
    stbi_set_flip_vertically_on_load(1);
    // 变量传 ptr
    if (!empty)
    {
        local_buffer = stbi_load(file_path.c_str(), &width, &height, &bit_per_pixel, 4); // rgba
    }

    GLCALL(glGenTextures(1, &renderer_id));
    GLCALL(glBindTexture(type, renderer_id));
    GLCALL(glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCALL(glTexImage2D(type, 0, GL_RGBA8, width, height, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
    GLCALL(glBindTexture(type, 0));
    if (local_buffer)
    {
        stbi_image_free(local_buffer);
    }
}

Texture2D::~Texture2D()
{
    GLCALL(glDeleteTextures(1, &renderer_id));
}

//////////////// Texture3D
Texture3D::Texture3D(const std::string &file_path, const bool empty)
    : Texture(GL_TEXTURE_3D), local_buffer(nullptr), file_path(file_path),
      width(0), height(0)
{
    // temp hardcode data
    const unsigned int w = 256;
    const unsigned int h = 256;
    const unsigned int d = 255;
    const char *filename = "../resource/textures/head256.raw";
    FILE *fp;
    size_t size = w * h * d;
    local_buffer = new unsigned char[size]; // 8bit
    if (!(fp = fopen(filename, "rb")))
    {
        std::cout << "Error: opening .raw file failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "OK: open .raw file successed" << std::endl;
    }
    size_t file_size = fread(local_buffer, sizeof(unsigned char), size, fp);
    std::cout << "read file size ? " << file_size << std::endl;

    fclose(fp);

    GLCALL(glGenTextures(1, &renderer_id));
    // bind 3D texture target
    GLCALL(glBindTexture(type, renderer_id));
    GLCALL(glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_REPEAT));
    // pixel transfer happens here from client to OpenGL server
    GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    // 3D 纹理的 internal format GL_INTENSITY => R = G = B = A 都是 I
    // GL_LUMINANCE => R = G = B, A = 1.0  不过这两个貌似都不能用??
    // Luminance is a greyscale image with alpha set to 1. Intensity is a greyscale image with alpha set to the greyscale value.
    GLCALL(glTexImage3D(type, 0, GL_RGB8, w, h, d, 0, GL_RED, GL_UNSIGNED_BYTE, local_buffer));
    // GLCALL(glTexImage3D(type, 0, GL_INTENSITY, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, local_buffer));

    delete[] local_buffer;
    local_buffer = nullptr;
}

Texture3D::~Texture3D()
{
    GLCALL(glDeleteTextures(1, &renderer_id));
}
