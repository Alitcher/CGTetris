#ifndef COLORS_H
#define COLORS_H
#include <glm/glm.hpp>

const glm::vec4 COLOR_PINK(1.0f, 0.545f, 0.718f, 1.0f); // #FF8BB7
const glm::vec4 COLOR_RED(1.0f, 0.545f, 0.545f, 1.0f); // #FF8B8B 
const glm::vec4 COLOR_ORANGE(1.0f, 0.792f, 0.545f, 1.0f); // #FF8B8B 
const glm::vec4 COLOR_YELLOW(1.0f, 0.992f, 0.545f, 1.0f); // #FFFD8B  
const glm::vec4 COLOR_GREEN(0.756f, 1.0f, 0.545f, 1.0f); // #C1FF8B  
const glm::vec4 COLOR_BLUE(0.545f, 1.0f, 1.0f, 1.0f); // #8BFFFF  
const glm::vec4 COLOR_NAVY(0.353f, 0.424f, 0.663f, 1.0f); // #5A6CA9  
const glm::vec4 COLOR_VIOLET1(0.545f, 0.627f, 1.0f, 1.0f); // #8BA0FF  
const glm::vec4 COLOR_VIOLET(0.847f, 0.545f, 1.0f, 1.0f); // #D88BFF  

glm::vec4 colors[] = {
    glm::vec4(1.0f, 0.545f, 0.718f, 1.0f), // pink
    glm::vec4(1.0f, 0.545f, 0.545f, 1.0f), // red
    glm::vec4(1.0f, 0.792f, 0.545f, 1.0f), // orange
    glm::vec4(1.0f, 0.992f, 0.545f, 1.0f), // yellow
    glm::vec4(0.756f, 1.0f, 0.545f, 1.0f), // green
    glm::vec4(0.545f, 1.0f, 1.0f, 1.0f), // blue
    glm::vec4(0.353f, 0.424f, 0.663f, 1.0f), // navy
    glm::vec4(0.545f, 0.627f, 1.0f, 1.0f), // violet1
    glm::vec4(0.847f, 0.545f, 1.0f, 1.0f) // violet
};


#endif