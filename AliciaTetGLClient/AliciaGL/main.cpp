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
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;


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

void CreateTetWindow4() 
{
    float x = 8;
    float y = 568.0;
    float offset = (0 * 80);
    Display display(SCREEN_WIDTH + 200, SCREEN_HEIGHT + 200, PROJECT_NAME);
    glViewport(0, 0, SCREEN_WIDTH + 200, SCREEN_HEIGHT + 200);

    //shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!display.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        testDrawTriangle();


        display.swapBuffers();
        display.pollEvents();
    }
}

int main() {
    
    CreateTetWindow4();
    return 0;
}

void testDrawTriangle() {
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f,
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // create and compile the shader program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    GLint colorLocation = glGetUniformLocation(shaderProgram, "uColor");
    glm::vec4 color(1.0f, 0.545f, 0.718f, 1.0f);
    glUniform4fv(colorLocation, 1, glm::value_ptr(color));

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0); // Unbind the shader program
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shaderProgram);
}



