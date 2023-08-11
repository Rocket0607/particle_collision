#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "VertexArray.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_rendererId);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_rendererId);
}

void VertexArray::addBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    unsigned int offset = 0;

    std::vector<VertexBufferElement> elements = layout.getElements();
    unsigned int stride = layout.getStride();
    for (int i = 0; i < elements.size(); i++) {
        glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalised, stride, (void*)offset);
        glEnableVertexAttribArray(i);
        offset += elements[i].count * elements[i].getTypeSize();
    }
}

void VertexArray::bind()
{
    glBindVertexArray(m_rendererId);
}

void VertexArray::unbind()
{
    glBindVertexArray(0);
}