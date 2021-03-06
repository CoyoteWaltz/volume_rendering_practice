#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include "utils.h"
#include <vector>

struct VertexBufferElement
{
    unsigned int type; // data type
    unsigned int count;
    unsigned char normalized;
    static unsigned int get_size_of_type(unsigned int type);
};

/**
 * 不同的 attribute 对应的位置 index
 * 每个 attribute 对应的 维度不同: count
 * 每个数据类型不同: template 决定
 * vertex array 取的步长也不一样
 */
class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;

public:
    VertexBufferLayout();

    template <typename T>
    void push(unsigned int count);

    template <>
    void push<float>(unsigned int count);

    template <>
    void push<unsigned int>(unsigned int count);

    template <>
    void push<unsigned char>(unsigned int count);

    inline const std::vector<VertexBufferElement> &get_elements() const
    {
        return elements;
    }

    inline unsigned int get_stride() const
    {
        // std::cout << "stride: " << stride << std::endl;
        return stride;
    }
};

#endif