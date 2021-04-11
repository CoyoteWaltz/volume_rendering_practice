#include "vertex_buffer.h"
#include "renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &renderer_id)); // 生成 1 个 buffer 给他个 point
    // 选择这个 buffer 就是 binding 操作 指明类型
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_id));
    // 接着给他 一点数据 和大小 usage GL_STATIC_DRAW 修改一次数据用好多次_画画用
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &renderer_id));
}

void VertexBuffer::bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_id));
}
void VertexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}