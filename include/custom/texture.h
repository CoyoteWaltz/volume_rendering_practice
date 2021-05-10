#ifndef TEXTURE_H
#define TEXTURE_H

#include "utils.h"
#include <string>

class Texture
{
private:
    unsigned int renderer_id;
    std::string file_path;
    unsigned char *local_buffer;
    int width;
    int height;
    int bit_per_pixel;

public:
    Texture(const std::string &file_path, const bool empty = false);
    ~Texture();

    void bind(unsigned int slot = 0) const; // texture slot
    void unbind();

    inline int get_width() const { return width; }
    inline int get_height() const { return height; }
};

#endif