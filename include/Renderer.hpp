#pragma once

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer
{
private:

public:
    Renderer();
    ~Renderer();
    void draw(VertexArray& va, IndexBuffer& ib, Shader& shader);
};