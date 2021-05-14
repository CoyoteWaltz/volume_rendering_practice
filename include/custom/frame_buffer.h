#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "utils.h"

class FrameBuffer
{
private:
    unsigned int renderer_id;
    unsigned int color_buffer_id;
    unsigned int depth_buffer_id;

public:
    FrameBuffer(const unsigned int width, const unsigned int height);
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

    void bind_color_buffer_to_texture(const int slot = 0) const;
    void unbind_color_buffer_to_texture() const;
};

#endif