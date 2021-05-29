#include "custom/texture.h"
#include "vendor/stb_image.h"

Texture1D::Texture1D(const std::string &file_path, const bool empty)
    : Texture(GL_TEXTURE_1D), local_buffer(nullptr), file_path(file_path),
      length(0)
{
    if (empty)
    {
        return;
    }
    std::ifstream file(file_path, std::ifstream::in);
    if (!file)
    {
        std::cerr << "[LOAD::Error::TEXTURE1D opening file: " << file_path << std::endl;
        exit(EXIT_FAILURE);
    }

    const int MAX_CNT = 10000;
    local_buffer = new unsigned char[MAX_CNT];
    file.read(reinterpret_cast<char *>(local_buffer), MAX_CNT);

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

std::vector<std::string> read_dir_paths(const std::string &dir_path)
{
    std::vector<std::string> paths;
    for (const auto &entry : std::filesystem::directory_iterator(dir_path))
    {
        paths.push_back(entry.path());
    }
    std::sort(paths.begin(), paths.end());
    return paths;
}

inline bool is_tiff(const std::string &file)
{
    return file.substr(file.size() - 4, file.size()).compare(".tif") == 0;
}

inline bool is_raw(const std::string &file)
{
    return file.substr(file.size() - 4, file.size()).compare(".raw") == 0;
}

void read_single_tiff(TIFF *tif, unsigned char *buffer, uint32_t width, uint32_t height, int current_depth)
{
    uint32_t pixels = width * height;
    uint32_t *raster = (uint32_t *)_TIFFmalloc(pixels * sizeof(uint32_t));

    uint32_t *scan_line = nullptr;
    if (raster != NULL)
    {
        if (TIFFReadRGBAImage(tif, width, height, raster, 0))
        {
            for (size_t i = 0; i < pixels; i++)
            {
                uint32_t value = raster[i];
                int low = (unsigned char)(value);
                buffer[current_depth * pixels + i] = low;
            }
        }
        _TIFFfree(raster);
    }
}

//////////////// Texture3D
Texture3D::Texture3D(const std::string &file_path)
    : Texture(GL_TEXTURE_3D), local_buffer(nullptr), file_path(file_path),
      width(0), height(0), depth(0)
{
    std::cout << "loading volume file/directory [" << file_path << "]" << std::endl;
    const auto &file_entry = std::filesystem::directory_entry(file_path);
    // 是否是 dir
    if (!file_entry.exists())
    {
        std::cout << "[Load volume failed!]: path not exist" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (file_entry.is_directory())
    {
        // 处理多个片
        std::cout << "handle dir..." << std::endl;
        const std::vector<std::string> files = read_dir_paths(file_path);
        depth = files.size();
        if (depth > 0)
        {
            const std::string &first = files[0];
            if (is_tiff(first))
            {
                TIFF *tif = TIFFOpen(first.c_str(), "r");
                if (!tif)
                {
                    std::cout << "open file failed! >>> " << first << std::endl;
                    exit(EXIT_FAILURE);
                }
                TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
                TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
                TIFFClose(tif);
            }
        }
        else
        {
            std::cout << "[Load volume failed!]: No files in " << file_path << std::endl;
            exit(EXIT_FAILURE);
        }
        uint32_t total = width * height * depth;
        local_buffer = new unsigned char[total];
        for (size_t i = 0; i < files.size(); ++i)
        {
            // 是否是 tiff 文件
            const std::string &file = files[i];
            if (is_tiff(file))
            {
                TIFF *tif = TIFFOpen(file.c_str(), "r");
                if (!tif)
                {
                    std::cout << "[Load volume failed!]: Open file failed! >>> " << file << std::endl;
                    exit(EXIT_FAILURE);
                }
                read_single_tiff(tif, local_buffer, width, height, i);
            }
        }
    }
    else
    {
        // 处理单文件
        if (is_tiff(file_path))
        {
            TIFF *tif = TIFFOpen(file_path.c_str(), "r");
            if (!tif)
            {
                std::cout << "[Load volume failed!]: Open file failed! >>> " << file_path << std::endl;
                exit(EXIT_FAILURE);
            }
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            depth = 0;
            do
            {
                depth++;
            } while (TIFFReadDirectory(tif)); // go next slice
            TIFFClose(tif);
            // 再次打开 读数据
            tif = TIFFOpen(file_path.c_str(), "r");
            uint32_t total = width * height * depth;
            local_buffer = new unsigned char[total];
            int dircount = 0;
            do
            {
                read_single_tiff(tif, local_buffer, width, height, dircount);
                dircount++;
            } while (TIFFReadDirectory(tif)); // go next slice
            TIFFClose(tif);
        }
        else if (is_raw(file_path))
        {
            width = 256;
            height = 256;
            depth = 256;
            FILE *fp;
            size_t size = width * height * depth;
            local_buffer = new unsigned char[size]; // 8bit
            if (!(fp = fopen(file_path.c_str(), "rb")))
            {
                std::cout << "[Load volume failed!]: Opening .raw file failed" << std::endl;
                exit(EXIT_FAILURE);
            }

            size_t file_size = fread(local_buffer, sizeof(unsigned char), size, fp);
            fclose(fp);
        }
        else
        {
            std::cout << "[Load volume failed!]:other file format not supported..." << std::endl;
            exit(EXIT_FAILURE);
        }
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
    GLCALL(glTexImage3D(type, 0, GL_RGB8, width, height, depth, 0, GL_RED, GL_UNSIGNED_BYTE, local_buffer));

    delete[] local_buffer;
    local_buffer = nullptr;
    std::cout << "loading volume file/directory [" << file_path << "] done." << std::endl;
    std::cout << "At size of " << width << " * " << height << " * " << depth << std::endl;
}

Texture3D::~Texture3D()
{
    GLCALL(glDeleteTextures(1, &renderer_id));
}
