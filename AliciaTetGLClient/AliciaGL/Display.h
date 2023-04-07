#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Display
{
public:
    Display(int width, int height, const std::string& title);
    virtual ~Display();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();

protected:
private:
    GLFWwindow* window_;

    Display(const Display& display) {}
    void operator=(const Display& display) {}
};

#endif
