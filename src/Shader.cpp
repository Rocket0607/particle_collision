#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <filesystem>

#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexString;
    std::string fragmentString;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    // ensure the ifstreams can throw exceptions
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        // create string streams to get file contents
        std::stringstream vertexStream;
        std::stringstream fragmentStream;
        // place contents of file buffer into string streams
        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();
        // convert streams to strings
        vertexString = vertexStream.str();
        fragmentString = fragmentStream.str();
        // close files
        vertexFile.close();
        fragmentFile.close();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl << e.what() << std::endl;
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
        std::cout << "Current directory is: " << std::filesystem::current_path() << std::endl;
    }
    // convert std::strings to char* for OpenGL
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    // error checking vars for when shaders are created
    int  success;
    char infoLog[512];
    // creating the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // creating the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // creating and linking final shader program (using ID property)
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // delete attached shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() 
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}