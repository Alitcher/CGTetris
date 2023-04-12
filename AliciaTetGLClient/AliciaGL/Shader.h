#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    GLuint ID;
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource); // Add this line
    ~Shader();
    void bind();

private:
    void checkCompileErrors(GLuint shader, const char* type);
};


#endif // SHADER_H