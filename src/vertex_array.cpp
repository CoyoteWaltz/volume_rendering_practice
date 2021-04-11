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
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto &element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(
            i,
            element.count,
            element.type,
            element.normalized,
            layout.get_stride(),
            (const void *)offset));
        offset += element.count * VertexBufferElement::get_size_of_type(element.type);
    }
}