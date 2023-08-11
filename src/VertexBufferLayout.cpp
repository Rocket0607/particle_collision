#include "VertexBufferLayout.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <vector>

VertexBufferLayout::VertexBufferLayout(){}
VertexBufferLayout::~VertexBufferLayout(){}

void VertexBufferLayout::pushf(unsigned int count, unsigned int normalised)
{
    m_elements.push_back({ GL_FLOAT, count, normalised });
    m_stride += count * sizeof(float);
}


std::vector<VertexBufferElement> VertexBufferLayout::getElements()
{
    return m_elements;
}

unsigned int VertexBufferLayout::getStride()
{
    return m_stride;
}