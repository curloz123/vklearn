#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <fmt/base.h>

int main()
{
    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // create window object
    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Vulkan", nullptr,nullptr);
    if (window == NULL)
    {
        fmt::println("Could not initialize window, exiting");
        return 1;
    }

    // game loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll all the events
        glfwPollEvents();

        // check whether escape key is pressed or not
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            // If true, set glfwWindowShouldClose to true
            glfwSetWindowShouldClose(window, true);
        }
    }

    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}