#include "vertex_buffer_layout.h"

unsigned int VertexBufferElement::get_size_of_type(unsigned int type)
{
    unsigned int size;
    if (type == GL_FLOAT)
    {
        size = sizeof(GLfloat);
    }
    else if (type == GL_UNSIGNED_INT)
    {
        size = sizeof(GLuint);
    }
    else if (type == GL_UNSIGNED_BYTE)
    {
        size = sizeof(GLubyte);
    }
    else
    {
        ASSERT(false);
    }
    return size;
    // return 0;
}

VertexBufferLayout::VertexBufferLayout()
    : stride(0)
{
}

template <typename T>
void VertexBufferLayout::push(unsigned int count)
{
    static_assert(true); // 禁止编译
}

template <>
void VertexBufferLayout::push<float>(unsigned int count)
{
    elements.push_back({GL_FLOAT, count, GL_FALSE});
    stride += count * VertexBufferElement::get_size_of_type(GL_FLOAT);
}

template <>
void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT);
}

template <>
void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE);
}
