#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a window
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // Print some information about the OpenGL context
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}
