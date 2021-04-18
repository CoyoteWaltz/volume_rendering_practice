#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray
{
private:
    unsigned int renderer_id;

public:
    VertexArray(); // constructor
    ~VertexArray();                                   // deconstructor

    void add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

    void bind() const;
    void unbind() const;
};

#endif