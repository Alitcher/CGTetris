#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;

wstring shapes[8];

string project_name = "Alicia TetrisGL";
const int window_width = 400;
const int window_height = 600;

void setupTetrominos() {
    shapes[0].append(L"..X...X...X...X.");
    shapes[1].append(L"..X..XX...X.....");
    shapes[2].append(L".....XX..XX.....");
    shapes[3].append(L"..X..XX..X......");
    shapes[4].append(L".X...XX...X.....");
    shapes[5].append(L".X...X...XX.....");
    shapes[6].append(L"..X...X..XX.....");
    shapes[7].append(L"X...X...X...XXX.");
}


void drawSquare(float x, float y, float size) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();
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

int main() {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, project_name.c_str(), nullptr, nullptr);

    if (!window) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 1);

    setupTetrominos();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1, 1, 1);
        for (int i = 0; i < 7; ++i) {
            drawTetromino(shapes[i], 50 + i * 80, 200, 20);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
