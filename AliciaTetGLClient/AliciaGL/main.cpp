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



void setBoard() 
{
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"X..........X");
    board.append(L"XXXXXXXXXXXX");

    cout << board.size() << endl;

}

void setupTetrominos() {
    shapes[0].append(L"..X.");
    shapes[0].append(L"..X.");
    shapes[0].append(L"..X.");
    shapes[0].append(L"..X.");

    shapes[1].append(L"..X.");
    shapes[1].append(L".XX.");
    shapes[1].append(L"..X.");
    shapes[1].append(L"....");

    shapes[2].append(L"....");
    shapes[2].append(L".XX.");
    shapes[2].append(L".XX.");
    shapes[2].append(L"....");

    shapes[3].append(L"..X.");
    shapes[3].append(L".XX.");
    shapes[3].append(L".X..");
    shapes[3].append(L"....");

    shapes[4].append(L".X..");
    shapes[4].append(L".XX.");
    shapes[4].append(L"..X.");
    shapes[4].append(L"....");

    shapes[5].append(L".X..");
    shapes[5].append(L".X..");
    shapes[5].append(L".XX.");
    shapes[5].append(L"....");

    shapes[6].append(L"..X.");
    shapes[6].append(L"..X.");
    shapes[6].append(L".XX.");
    shapes[6].append(L"....");

    shapes[7].append(L"X...");
    shapes[7].append(L"X...");
    shapes[7].append(L"X...");
    shapes[7].append(L"XXX.");

}




void drawCircle(float x, float y, float radius) {
    int segments = 12;
    float* vertices = new float[static_cast<size_t>(segments) * 2];

    for (int i = 0; i < segments; i++) {
        float angle = (float)i / (float)segments * 2.0f * 3.14159f;
        float dx = cosf(angle) * radius;
        float dy = sinf(angle) * radius;
        vertices[i * 2] = x + dx;
        vertices[i * 2 + 1] = y + dy;
    }

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * segments * 2, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);



    glDrawArrays(GL_TRIANGLE_FAN, 0, segments);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    delete[] vertices;
}

GLuint shaderProgram;

void drawSquare(float x, float y, float size, const glm::vec4& color) {
    float vertices[] = {
        x, y,
        x + size, y,
        x, y + size,
        x + size, y + size
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glUseProgram(shaderProgram);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glUseProgram(0); // Unbind the shader program
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}




const float gap = 2;

void drawTetromino(const wstring& tetro, float x, float y, float size) {
    glm::vec4 pink(1.0f, 0.5f, 0.5f, 1.0f);

    for (int i = 0; i < 16; ++i) {
        if (tetro[i] == L'X') {
            float xPos = x + (i % 4) * (size + gap);
            float yPos = y + (i / 4) * (size + gap);
            drawSquare(xPos, yPos, size, pink);
        }
    }
}


float size = 20.0;

void drawBoard()
{
    float x = 30.0;
    float y = 150.0;
    glm::vec4 purple(0.5f, 0.0f, 0.5f, 1.0f);

    for (int i = 0; i < board.size(); ++i) {
        if (board.at(i) == L'X') {
            int row = i / 12;
            int col = i % 12;
            float xPos = x + col * (::size + gap);
            float yPos = y + row * (::size + gap);
            //std::cout << "xPos: " << xPos << ", yPos: " << yPos << std::endl;
            drawSquare(xPos, yPos, ::size, purple);

        }
    }
}

extern void maintetris();

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

        drawBoard();
        drawTetromino(shapes[0], x, y, ::size);


        display.swapBuffers();
        display.pollEvents();
    }
}

int main() {
    setBoard();
    setupTetrominos();
    
    CreateTetWindow4();
    //maintetris();
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

    glUseProgram(shaderProgram);
    GLint colorLocation = glGetUniformLocation(shaderProgram, "uColor");
    glm::vec4 color(0.5f, 0.0f, 0.5f, 1.0f);
    glUniform4fv(colorLocation, 1, glm::value_ptr(color));

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0); // Unbind the shader program
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

glm::vec2 screenToNDC(float x, float y, float width, float height) {
    float normalizedX = (2 * x / width) - 1;
    float normalizedY = (2 * y / height) - 1;
    return glm::vec2(normalizedX, normalizedY);
}

float screenSizeToNDC(float size, float screenSize) {
    return 2 * size / screenSize;
}

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


void setShaderColor(GLuint shaderProgram, const char* uniformName, const glm::vec4& color) {
    GLint colorLocation = glGetUniformLocation(shaderProgram, uniformName);
    glUseProgram(shaderProgram);
    glUniform4fv(colorLocation, 1, glm::value_ptr(color));
    glUseProgram(0);
}

GLuint compileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}



//for (int i = 0; i < 8; ++i) {
//    drawTetromino(shapes[i], x + i * 80, y, ::size);
//}