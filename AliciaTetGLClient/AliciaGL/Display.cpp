#include "Display.h"
#include <iostream>

Display::Display(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window_);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0, 0, 0, 1);
}

Display::~Display() {
    glfwTerminate();
}

bool Display::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Display::swapBuffers() {
    glfwSwapBuffers(window_);
}

void Display::pollEvents() {
    glfwPollEvents();
}

GLFWwindow* Display::getWindow() const {
    return window_;
}