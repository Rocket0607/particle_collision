#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"

#include <iostream>
#include <cmath>

void on_particle_collision(float m_1, float m_2, float x_1, float x_2, float y_1, float y_2, glm::vec2 v_1, glm::vec2 v_2, glm::vec3* vel_1, glm::vec3* vel_2, float dt);
void glfwError(int id, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    // glfw: init and configure
    glfwSetErrorCallback(&glfwError);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Collision Simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load OpengGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    const float origin_x = 0.0f;
    const float origin_y = 0.0f;
    const float radius = 0.1f;
    const int n_points = 360;
    const float theta_points = 360.0f / n_points;
    glm::vec3 points[n_points];
    for (int i = 0; i < n_points; i++) {
        float theta_current = i * theta_points;
        float x = cos(glm::radians(theta_current)) * radius + origin_x;
        float y = sin(glm::radians(theta_current)) * radius + origin_y;
        float z = 0.0;
        points[i] = glm::vec3(x, y, z);
    }

    const int n_triangles = n_points - 2;
    unsigned int vertices[n_triangles * 3];
    for (int i = 0; i < n_triangles; i++) {
        vertices[i * 3 + 0] = 0;
        vertices[i * 3 + 1] = i + 1;
        vertices[i * 3 + 2] = i + 2;
    }

    Renderer renderer = Renderer();
    Shader shader = Shader("src\\shaders\\shader.vs", "src\\shaders\\shader.fs");
    VertexArray va = VertexArray();
    VertexBuffer vb = VertexBuffer((void*)points, sizeof(glm::vec3) * n_points);
    IndexBuffer ib = IndexBuffer(vertices, n_triangles * 3);
    VertexBufferLayout layout = VertexBufferLayout();
    layout.pushf(3, GL_FALSE);
    va.addBuffer(vb, layout);

    glm::mat4 translation = glm::mat4(1.0f);
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 vel = glm::vec3(0.5f, 0.3f, 0.0f);
    glm::vec3 accel = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 translation2 = glm::mat4(1.0f);
    glm::vec3 pos2 = glm::vec3(0.0f, 0.5f, 0.0f);
    glm::vec3 vel2 = glm::vec3(0.5f, 0.1f, 0.0f);
    glm::vec3 accel2 = glm::vec3(0.0f, 0.0f, 0.0f);

    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window)) {

        // Calculate delta time (dt)
        GLfloat curr_frame = glfwGetTime();
        delta_time = curr_frame - last_frame;
        last_frame = curr_frame;

        // wall collision detection
        if (pos.x < -1 || pos.x > 1) {
            vel.x *= -1;
        }
        if (pos.y < -1 || pos.y > 1) {
            vel.y *= -1;
        }

        if (pos2.x < -1 || pos2.x > 1) {
            vel2.x *= -1;
        }
        if (pos2.y < -1 || pos2.y > 1) {
            vel2.y *= -1;
        }

        // particle collision detection
        if (abs(pos.x - pos2.x) < 0.2f && abs(pos.y - pos2.y) < 0.2f) {
            std::cout << "old vel 1: " << vel.x << ", " << vel.y << std::endl;
            std::cout << "old vel 2: " << vel2.x << ", " << vel2.y << std::endl;
            on_particle_collision(0.2f, 0.1f, pos.x, pos2.x, pos.y, pos2.y, glm::vec2(vel.x, vel.y), glm::vec2(vel2.x, vel2.y), &vel, &vel2, delta_time);
            std::cout << "new vel 1: " << vel.x << ", " << vel.y << std::endl;
            std::cout << "new vel 2: " << vel2.x << ", " << vel2.y << std::endl;
            std::cout << std::endl;
        }

        vel += accel * delta_time;
        pos += vel * delta_time;
        
        vel2 += accel2 * delta_time;
        pos2 += vel2 * delta_time;

        translation = glm::translate(glm::mat4(1.0f), pos);
        translation2 = glm::translate(glm::mat4(1.0f), pos2);

        int translationLoc = glGetUniformLocation(shader.ID, "translation");

        processInput(window);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation));
        renderer.draw(va, ib, shader);

        glUniformMatrix4fv(translationLoc, 1, GL_FALSE, glm::value_ptr(translation2));
        renderer.draw(va, ib, shader);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void on_particle_collision(float m_1, float m_2, float x_1, float x_2, float y_1, float y_2, glm::vec2 v_1, glm::vec2 v_2, glm::vec3* vel1, glm::vec3* vel2, float dt)
{
    glm::vec2 C_1 = glm::vec2(x_1, y_1);
    glm::vec2 C_2 = glm::vec2(x_2, y_2);
    glm::vec2 v_1_prime = v_1 - (float)((2*m_2/(m_1 + m_2))*(glm::dot(v_1-v_2, C_1-C_2)/pow(glm::length(C_1-C_2), 2))) * (C_1-C_2);
    glm::vec2 v_2_prime = v_2 - (float)((2*m_1/(m_1 + m_2))*(glm::dot(v_2-v_1, C_2-C_1)/pow(glm::length(C_2-C_1), 2))) * (C_2-C_1);
    *vel1 = glm::vec3(v_1_prime.x, v_1_prime.y, 0.0f);
    *vel2 = glm::vec3(v_2_prime.x, v_2_prime.y, 0.0f);
}

// error: callback to print errors
void glfwError(int id, const char* description)
{
  std::cout << description << std::endl;
}

// process input:  quary GLFW for relevant key status and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

// window: when window size is changed, this function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}