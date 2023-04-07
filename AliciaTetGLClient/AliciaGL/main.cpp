#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Display.h"

using namespace std;

wstring shapes[8];

string PROJECT_NAME = "Alicia TetrisGL";
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;

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


void drawSquare(float x, float y, float size) {
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

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
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

const float gap = 2;

void drawTetromino(const wstring& tetro, float x, float y, float size) {
    for (int i = 0; i < 16; ++i) {
        if (tetro[i] == L'X') {
            float xPos = x + (i % 4) * (size + gap);
            float yPos = y + (i / 4) * (size + gap);
            drawSquare(xPos, yPos, size);
        }
    }
}


void CreateTetWindow4() 
{
    Display display(SCREEN_WIDTH + 200, SCREEN_HEIGHT + 200, PROJECT_NAME);

    setupTetrominos();

    while (!display.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1, 1, 1);
        for (int i = 0; i < 8; ++i) {
            drawTetromino(shapes[i], 50 + i * 80, 200, 20);
        }

        display.swapBuffers();
        display.pollEvents();
    }
}

int main() {
    CreateTetWindow4();
    return 0;
}
