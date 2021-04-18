#include "custom/index_buffer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : count(count)
{
    // in case type
    ASSERT(sizeof(unsigned int) == sizeof(GLint));
    GLCALL(glGenBuffers(1, &renderer_id)); // 生成 1 个 buffer 给他个 point
    // 选择这个 buffer 就是 binding 操作 指明类型
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id));
    // 接着给他 一点数据 和大小 usage GL_STATIC_DRAW 修改一次数据用好多次_画画用
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &renderer_id));
}

void IndexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id));
}
void IndexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::get_count() const
{
    return count;
}