#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "utils.h"

class FrameBuffer
{
private:
    unsigned int renderer_id;
    unsigned int color_buffer_id;
    unsigned int _buffer_id;

public:
    FrameBuffer(const unsigned int *data, unsigned int count); // constructor int data
    ~FrameBuffer();                                  // deconstructor

    unsigned int get_count() const;

    void bind() const;
    void unbind() const;
};

#endif