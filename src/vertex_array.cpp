#include "vertex_array.h"
#include "vertex_buffer_layout.h"

VertexArray::VertexArray()
{
    GLCALL(glGenVertexArrays(1, &renderer_id));
    GLCALL(glBindVertexArray(renderer_id));
}

VertexArray::~VertexArray()
{
    GLCALL(glDeleteVertexArrays(1, &renderer_id));
}

void VertexArray::bind() const
{
    GLCALL(glBindVertexArray(renderer_id));
}

void VertexArray::unbind() const
{
    GLCALL(glBindVertexArray(0));
}

void VertexArray::add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.bind();
    // setup layouts
    const auto &elements = layout.get_elements();
    unsigned int offset = 0; // 从 0 开始 按照 stride 取 attribute 数据
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto &element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(
            i,             // location attribute 的 index
            element.count, // 数据个数
            element.type,  // 数据类型
            element.normalized,
            layout.get_stride(), // 每次取的 byte 间隔
            (const void *)offset));
        offset += element.count * VertexBufferElement::get_size_of_type(element.type);
    }
}