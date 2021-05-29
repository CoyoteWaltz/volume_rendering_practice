#ifndef TEXTURE_H
#define TEXTURE_H

#include "utils.h"
#include "tiffio.h"
#include "tiffio.hxx"
#include <string>
#include <vector>
#include <fstream>

class Texture
{
private:
protected:
    unsigned int type; // GL_TEXTURE_2D GL_TEXTURE_3D ...
    unsigned int renderer_id;
    int bit_per_pixel; // 通道数

public:
    Texture(unsigned int t)
        : type(t), renderer_id(0), bit_per_pixel(0)
    {
    }
    virtual ~Texture()
    {
    }
    void unbind()
    {
        GLCALL(glBindTexture(type, 0));
    }
    void bind(unsigned int slot) const
    {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot)); // texture slot 最大 32 个
        GLCALL(glBindTexture(type, renderer_id));
    }
    inline unsigned int get_id() const { return renderer_id; }
};

class Texture1D : public Texture
{
private:
    std::string file_path;
    unsigned char *local_buffer;
    int length;

public:
    Texture1D(const std::string &file_path, const bool empty = false);
    ~Texture1D();

    inline int get_length() const { return length; }
};

class Texture2D : public Texture
{
private:
    std::string file_path;
    unsigned char *local_buffer;
    int width;
    int height;

public:
    Texture2D(const std::string &file_path, const bool empty = false, unsigned int w = 0, unsigned int h = 0);
    ~Texture2D();

    inline int get_width() const { return width; }
    inline int get_height() const { return height; }
};

class Texture3D : public Texture
{
private:
    std::string file_path;
    unsigned char *local_buffer;
    int width;
    int height;
    int depth;

public:
    Texture3D(const std::string& file_path);
    ~Texture3D();

    inline int get_width() const { return width; }
    inline int get_height() const { return height; }
    inline int get_depth() const { return depth; }
};

#endif