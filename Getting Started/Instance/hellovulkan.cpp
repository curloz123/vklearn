#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <cstdint>
#include <vector>
#include <cstring>

// Add vulkan headers
#include <vulkan/vulkan.h>

#include "initializers.hpp"

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
                printf("Could not initialize window, exiting\n");
                return 1;
        }

        // Window creation above


        uint32_t glfwExtensionCount = 0;
        // Get all the required extensions.
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Create seperate for storing required extensions
        std::vector<const char *> requiredExtensions;
        for (int i=0; i<glfwExtensionCount; ++i)
        {
                requiredExtensions.emplace_back(glfwExtensions[i]);
        }
       
        VkInstance instance;
        createInstance(&instance, requiredExtensions);


        // game loop below

        // game loop
        //while (!glfwWindowShouldClose(window))
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

        vkDestroyInstance(instance, nullptr);
        
        glfwDestroyWindow(window);
        glfwTerminate();

        printf("Exiting....\n");

        return 0;
}