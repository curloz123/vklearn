#pragma once 

#include <vulkan/vulkan.h>
#include <vector>
#include <stdio.h>
#include <cstring>

// Instance Creation
bool createInstance(VkInstance *pInstance, const std::vector<const char*> &requiredExtensions, const std::vector<const char*> &validationLayers)
{
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


// debug messenger
VKAPI_ATTR VkBool32 VKAPI_CALL printMessage(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pMessageData,
    void* pUserData) 
{
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
                printf("Validation Layer: %s\n", pMessageData->pMessage);

        return VK_FALSE;
}

bool createDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
        VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
        messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        messengerInfo.pfnUserCallback = printMessage;
        messengerInfo.pUserData = nullptr; // Optional

        PFN_vkCreateDebugUtilsMessengerEXT createMessenger = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (createMessenger == NULL)
        {
                printf("Could not find the debug messenger creator function, Exiting....\n");
                return false;
        }

        if (createMessenger(instance, &messengerInfo, nullptr, pDebugMessenger) != VK_SUCCESS)
        {
                printf("Could not create debug messenger, Exiting...\n");
                return false;
        }
        printf("Created Debug Messenger\n");
        return true;
}
void destroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger)
{
        PFN_vkDestroyDebugUtilsMessengerEXT destroyMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (destroyMessenger != NULL)
        {
                destroyMessenger(instance, debugMessenger, nullptr);
        }
}