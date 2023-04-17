#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

class Shader {
public:
    Shader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource);
    ~Shader();
    void use();
    void setUniform(const char* name, glm::vec4 value);
    GLuint shaderProgram;
    GLint colorLocation;

    GLuint createShader(GLenum shaderType, const GLchar* shaderSource);
    GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource);
};


#endif // SHADER_H