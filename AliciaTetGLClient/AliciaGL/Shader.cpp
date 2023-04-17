#include "Shader.h"

Shader::Shader(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
    shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    colorLocation = glGetUniformLocation(shaderProgram, "uColor");
}

Shader::~Shader() {
    std::cout << "Render stopped..." << std::endl;
    glDeleteProgram(shaderProgram);
}

void Shader::use() {
    glUseProgram(shaderProgram);
}

void Shader::setUniform(const char* name, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(shaderProgram, name), 1, glm::value_ptr(value));
}

GLuint Shader::createShader(GLenum shaderType, const GLchar* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint Shader::createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
    GLuint shaderProgram = glCreateProgram();
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}