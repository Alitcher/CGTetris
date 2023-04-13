#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Display.h"

using namespace std;

wstring shapes[8];
wstring board;

string PROJECT_NAME = "Alicia TetrisGL";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 1200;


const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;

void main() {
    FragColor = uColor;
}

)";

const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

void testDrawTriangle();
void deleteBuffers(GLuint& vao, GLuint& vbo);
GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource);
void createBuffers(GLuint& vao, GLuint& vbo, float* vertices, GLsizei verticesSize);
GLuint vao, vbo;
GLuint shaderProgram;
glm::vec4 colorPink(1.0f, 0.545f, 0.718f, 1.0f);
GLint colorLocation;

float triangle_vertices[] = {
    -0.05f, 0.0f,
     0.05f, 0.0f,
     0.0f,  0.05f,
};
void CreateTetWindow4()
{
    Display display(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool shouldDrawTriangle = true;
    shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    createBuffers(vao, vbo, triangle_vertices, sizeof(triangle_vertices));

    colorLocation = glGetUniformLocation(shaderProgram, "uColor");

    while (!display.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (shouldDrawTriangle) {
            testDrawTriangle();
        }

        display.swapBuffers();
        display.pollEvents();
    }
    deleteBuffers(vao,vbo);
    glDeleteProgram(shaderProgram);
}

int main() {

    CreateTetWindow4();
    return 0;
}

void createBuffers(GLuint& vao, GLuint& vbo, float* vertices, GLsizei verticesSize) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void deleteBuffers(GLuint& vao, GLuint& vbo) {
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

GLuint createShader(GLenum shaderType, const GLchar* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    return shader;
}

GLuint createShaderProgram(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {
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

void drawTriangle(glm::vec4 color) {
    glUseProgram(shaderProgram);
    glUniform4fv(colorLocation, 1, glm::value_ptr(color));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(0); // Unbind the shader program
    glBindVertexArray(0);
}

void testDrawTriangle() {

    drawTriangle(colorPink);
    // no delete buffers and shader program here?
}