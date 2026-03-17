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
#include "debugmessenger.hpp"

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
         // check layers support
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* requiredLayer : validationLayers)
        {
                bool layerFound = false;
                
                for (const VkLayerProperties &layerProperty : availableLayers)
                {
                        if (std::strcmp(requiredLayer, layerProperty.layerName) == 0)
                        {
                                layerFound = true;
                                break;    
                        }

                }
                
                if (layerFound == false)
                {
                        printf("Requested Layer: %s, Not available, Exiting....\n", requiredLayer);
                        return 1;
                }
                
        }
        printf("All requested layers are present\n");

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




        // game loop below

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
        destroyDebugMessenger(instance, debugMessenger);
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);
        glfwTerminate();

        printf("Exiting....\n");

        return 0;
}