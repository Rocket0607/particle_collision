#pragma once

#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned int normalised;

    unsigned int getTypeSize()
    {
        switch(type) {
            case GL_FLOAT: return 4;
        }
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride = 0;
public:
    VertexBufferLayout();
    ~VertexBufferLayout();
    void pushf(unsigned int count, unsigned int normalised);
    std::vector<VertexBufferElement> getElements();
    unsigned int getStride();
};