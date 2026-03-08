#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <fmt/base.h>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Vulkan", nullptr,nullptr);
    if (window == NULL)
    {
        fmt::println("Could not initialize window, exiting");
        return 1;
    }

    while (!glfwWindowShouldClose(window))
    {
        // Poll all the events
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}