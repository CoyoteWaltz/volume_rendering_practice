#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer
{
private:
    unsigned int renderer_id;
    unsigned int count;  // count: el number, size: bytes

public:
    IndexBuffer(const unsigned int *data, unsigned int count); // constructor int data
    ~IndexBuffer();                                  // deconstructor

    inline unsigned int get_count() const;

    void bind() const;
    void unbind() const;
};

#endif