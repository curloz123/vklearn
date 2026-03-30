#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <cstdint>
#include <vector>
#include <cstring>

// Add vulkan headers
#include <vulkan/vulkan.h>

#include "boilerplate.hpp"

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

        // Enabling validation layers
        const std::vector<const char*> validationLayers = 
        {
                "VK_LAYER_KHRONOS_validation"
        };

        // Instance creation
        uint32_t glfwExtensionCount = 0;
        // Get all the required extensions.
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Create seperate for storing required extensions
        std::vector<const char *> requiredExtensions;
        for (int i=0; i<glfwExtensionCount; ++i)
        {
                requiredExtensions.emplace_back(glfwExtensions[i]);
        }
        // debug messenger extension
        requiredExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        VkInstance instance;
        if (!createInstance(&instance, requiredExtensions, validationLayers))
                return 1;

        // Create Debug messenger after creating instance
         VkDebugUtilsMessengerEXT debugMessenger;
         if (!createDebugMessenger(instance, &debugMessenger))
                return 1;

        







        // Surface
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        {
                printf("Could not create window surface, exiting...\n");
                return 1;
        }
        printf("Created surface succesfully\n");












        // Physical Device
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        if (!selectPhysicalDevice(instance, surface, &physicalDevice))
        {
                printf("Exiting....\n");
                return 1;
        }
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        // Queue family handles
        uint32_t graphicsFamilyIndex = 0;
        uint32_t presentFamilyIndex  = 0;
        retrieveQueueFamilies(physicalDevice, surface, &graphicsFamilyIndex, &presentFamilyIndex);






        // Logical Device
        VkDevice device = VK_NULL_HANDLE;
        createDevice(physicalDevice, graphicsFamilyIndex, presentFamilyIndex, &device);

        // Retrieve queue handles
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        retrieveQueueIndexes(device, graphicsFamilyIndex, presentFamilyIndex, &graphicsQueue, &presentQueue);

        
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
        vkDestroyDevice(device, nullptr);       
        printf("Destroyed Device\n");
        vkDestroySurfaceKHR(instance, surface, nullptr);
        printf("Destroyed Surface\n");
        destroyDebugMessenger(instance, debugMessenger);
        printf("Destroyed Debug messenger\n");
        vkDestroyInstance(instance, nullptr);
        printf("Destroyed Instance\n");

        glfwDestroyWindow(window);
        glfwTerminate();

        printf("Exiting Success....\n");

        return 0;
}