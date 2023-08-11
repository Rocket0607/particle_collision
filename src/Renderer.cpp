#include "Renderer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

Renderer::Renderer(){}
Renderer::~Renderer(){}

void Renderer::draw(VertexArray& va, IndexBuffer& ib, Shader& shader)
{
    va.bind();
    ib.bind();
    shader.use();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
}
