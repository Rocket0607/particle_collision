#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
    unsigned int m_rendererId;
    unsigned int m_index;
public:
    VertexArray();
    ~VertexArray();
    void bind();
    void unbind();
    void addBuffer(VertexBuffer& vb, VertexBufferLayout& layout);
};