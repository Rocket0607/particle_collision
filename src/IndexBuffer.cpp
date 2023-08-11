#include "IndexBuffer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
:m_count(count)
{
    glGenBuffers(1, &m_rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_rendererId);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() const
{
    return m_count;
}