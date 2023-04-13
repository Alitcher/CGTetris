#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <iostream>

std::string PROJECT_NAME = "Alicia TetrisGL";
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
    uniform vec2 uTranslation;

    void main() {
        vec2 translatedPos = aPos + uTranslation;
        gl_Position = vec4(translatedPos, 0.0, 1.0);
}
)glsl";

#endif 