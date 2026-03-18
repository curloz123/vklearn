#pragma once 

#include <vulkan/vulkan.h>
#include <vector>
#include <stdio.h>
#include <cstring>

inline bool createInstance(VkInstance *pInstance, const std::vector<const char*> &requiredExtensions, const std::vector<const char*> &validationLayers)
{
        // Application Info
        VkApplicationInfo appInfo = {};

        // Define which type of object is this
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        // Name of our application
        appInfo.pApplicationName = "Hello vulkan";
        // Our application's version
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        // Our application's name
        appInfo.pEngineName = "Any name you want";
        // Which API version we want to use
        appInfo.apiVersion = VK_API_VERSION_1_3;





        // Instance info
        VkInstanceCreateInfo instanceInfo = {};

        // Struct type 
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        // Our application's info
        instanceInfo.pApplicationInfo = &appInfo;

        // query all available extensions
        uint32_t availabeExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &availabeExtensionCount, nullptr);
        std::vector<VkExtensionProperties> availabeExtensions(availabeExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &availabeExtensionCount, availabeExtensions.data());

        for (int i=0; i<requiredExtensions.size(); ++i)
        {
                bool found = false;
                for (int j=0; j<availabeExtensions.size(); ++j)
                {
                        if (std::strcmp(requiredExtensions[i], availabeExtensions[j].extensionName) == 0)
                        {
                                found = true;
                                break;
                        }

                }
                if (found == false)
                {
                        printf("Extension: %s Not available, Exiting....\n", requiredExtensions[i]);
                        return false;
                }
        }
        printf("All required extensions present\n");

        // Send info about extensions
        instanceInfo.enabledExtensionCount = requiredExtensions.size();
        instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

        // Enable layers
        instanceInfo.enabledLayerCount = validationLayers.size();
        instanceInfo.ppEnabledLayerNames = validationLayers.data();

        VkResult result = vkCreateInstance(&instanceInfo, nullptr, pInstance);
        if (result != VK_SUCCESS)
        {
                printf("Could not create instance, exiting....\n");
                return false;
        }
        printf("Created Instance\n");     
        return true;  
}
