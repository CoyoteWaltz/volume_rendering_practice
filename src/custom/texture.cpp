#include "custom/texture.h"
#include "vendor/stb_image.h"
#include "tiffio.h"
#include "tiffio.hxx"
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
Texture2D::Texture2D(const std::string &file_path, const bool empty,
                     unsigned int w, unsigned int h)
    : Texture(GL_TEXTURE_2D), local_buffer(nullptr), file_path(file_path),
      width(w), height(h)
{
    if (!empty)
    {
        std::cout << "GEN texture 2d " << file_path << std::endl;

        // flip vertically 图像 因为 OpenGL 的坐标是左下角 (0,0)
        // 图片是左上角 (0,0)
        stbi_set_flip_vertically_on_load(1);
        // 变量传 ptr
        local_buffer = stbi_load(file_path.c_str(), &width, &height, &bit_per_pixel, 4); // rgba

        GLCALL(glGenTextures(1, &renderer_id));
        GLCALL(glBindTexture(type, renderer_id));
        GLCALL(glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCALL(glTexImage2D(type, 0, GL_RGBA8, width, height, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
        if (local_buffer)
        {
            stbi_image_free(local_buffer);
        }
    }
    else
    {
        std::cout << "GEN empty texture 2d" << std::endl;
        // 生成 空 纹理对象
        GLCALL(glGenTextures(1, &renderer_id));
        GLCALL(glBindTexture(type, renderer_id));
        GLCALL(glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCALL(glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCALL(glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, 0));
        GLCALL(glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, 0));

        // internal type GL_RGBA16F 和我们在 glsl 中如何存的 有关
        GLCALL(glTexImage2D(type, 0, GL_RGBA16F, width, height, 0,
                            GL_RGBA, GL_FLOAT, NULL));
    }
    GLCALL(glBindTexture(type, 0)); // unbind texture
}

Texture2D::~Texture2D()
{
    GLCALL(glDeleteTextures(1, &renderer_id));
}

//////////////// Texture3D
Texture3D::Texture3D(const bool face)
    : Texture(GL_TEXTURE_3D), local_buffer(nullptr), file_path(file_path),
      width(0), height(0)
{
    // temp hardcode data
    unsigned int w = 256;
    unsigned int h = 256;
    unsigned int d = 255;
    if (face)
    {

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
    }
    else
    {
        w = 512;
        h = 512;
        d = 512;

        const char *filename = "../resource/textures/neuro.tif";
        TIFF *tif = TIFFOpen(filename, "r");
        if (!tif)
        {
            std::cout << "open file failed! >>> " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
        uint32_t width, height;
        uint32_t tileWidth, tileLength;
        uint32_t x, y;
        // uint32_t type;

        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileLength);
        // TIFFGetField(tif, TIFFTAG_DATATYPE, &type);
        std::cout << "TIFFTAG_IMAGEWIDTH " << width
                  << "TIFFTAG_IMAGELENGTH " << height
                  << std::endl;
        // std::cout << "type " << type << std::endl;

        int dircount = 0;
        uint32_t depth = 512;
        uint32_t total = width * height * depth;
        local_buffer = new unsigned char[total];
        do
        {
            dircount++;
            uint32_t pixels = width * height;
            uint32_t *raster = (uint32_t *)_TIFFmalloc(pixels * sizeof(uint32_t));

            uint32_t *scan_line = nullptr;
            if (raster != NULL)
            {
                if (TIFFReadRGBAImage(tif, width, height, raster, 0))
                {
                    // std::cout << "read success " << dircount << std::endl;
                    for (size_t i = 0; i < pixels; i++)
                    {
                        // std::cout << "data: " << i << " " << raster[i] << std::endl;
                        uint32_t value = raster[i];
                        int low = (unsigned char)(value);
                        local_buffer[(dircount - 1) * pixels + i] = low;
                    }
                }
                _TIFFfree(raster);
            }
        } while (TIFFReadDirectory(tif)); // go next slice
        TIFFClose(tif);
    }

    GLCALL(glGenTextures(1, &renderer_id));
    // bind 3D texture target
    GLCALL(glBindTexture(type, renderer_id));
    GLCALL(glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // s t r -> u v d
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
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
