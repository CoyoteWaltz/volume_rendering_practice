#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class VertexBuffer
{
private:
    unsigned int renderer_id;

public:
    VertexBuffer(const void *data, unsigned int size);  // constructor
    ~VertexBuffer(); // deconstructor

    void bind() const;
    void unbind() const;
};

#endif