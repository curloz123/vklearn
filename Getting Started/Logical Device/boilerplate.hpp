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
                        printf("Requested Layer: %s, Not available\n", requiredLayer);
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
                        printf("Extension: %s Not available\n", requiredExtensions[i]);
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
                printf("Could not create instance\n");
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
                printf("Could not find the debug messenger creator function\n");
                return false;
        }

        if (createMessenger(instance, &messengerInfo, nullptr, pDebugMessenger) != VK_SUCCESS)
        {
                printf("Could not create debug messenger\n");
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




// Physical Device

bool selectPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice *pPhysicalDevice)
{
        // selecting multiple GPU's
        uint32_t numPhysicalDevices = 0;
        vkEnumeratePhysicalDevices(instance, &numPhysicalDevices, nullptr);
        std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
        vkEnumeratePhysicalDevices(instance, &numPhysicalDevices, physicalDevices.data());

        uint32_t maxScore = 0;
        for (int i=0; i<physicalDevices.size(); ++i)
        {
                VkPhysicalDeviceProperties deviceProperties;
                vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
                printf("GPU: %s\nDriver version: %d.%d.%d\nMax API version: %d.%d.%d\n",
                        deviceProperties.deviceName,
                        VK_API_VERSION_MAJOR(deviceProperties.driverVersion),
                        VK_API_VERSION_MINOR(deviceProperties.driverVersion),
                        VK_API_VERSION_PATCH(deviceProperties.driverVersion),
                        VK_API_VERSION_MAJOR(deviceProperties.apiVersion),
                        VK_API_VERSION_MINOR(deviceProperties.apiVersion),
                        VK_API_VERSION_PATCH(deviceProperties.apiVersion)
                );

                // Check for Graphics and Present Support

                // Retrieve all queue families
                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, nullptr);
                std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, queueFamilies.data());
                
                bool graphicsSupport = false;
                VkBool32 presentSupport = false;
                for (int j = 0; j < queueFamilies.size(); ++j)
                {
                        // Graphics Support
                        if (queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                                graphicsSupport = true;
                        
                        // Present Support
                        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], i, surface, &presentSupport);


                        if (graphicsSupport && presentSupport)
                        break;
                }

                if (!graphicsSupport || !presentSupport)
                {
                        printf("Physical Device: %s, is not suitable\n", deviceProperties.deviceName);
                        continue;
                }
                if (deviceProperties.apiVersion < VK_API_VERSION_1_3)
                {
                        printf("Physical Device: %s, Does not support vulkan 1.3\n", deviceProperties.deviceName);
                        continue;
                }

                // this device's local score
                uint32_t deviceScore = 0;

                // First parameter - Checking for maximum image size 
                deviceScore += deviceProperties.limits.maxImageDimension2D;

                // Checking is the device is Discrete or not
                if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                        deviceScore += 10000;           // This is basically a no brainer. Discrete is always the fastest.

                // Final comparison with previous devices to see if current device has a better score
                if (deviceScore > maxScore)
                        *(pPhysicalDevice) = physicalDevices[i];
        }
        if (*pPhysicalDevice == VK_NULL_HANDLE)
        {
                printf("No Suitable GPU found\n");
                return false;
        }

        return true;
}
void retrieveQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t *pGraphicsFamily, uint32_t *pPresentFamily)
{
        // Retrieve all queue families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
               
        bool graphicsFamilyFound = false;
        VkBool32 presentFamilyFound = false;
        for (int i = 0; i < queueFamilies.size(); ++i)
        {
                // Graphics Support
                if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                        *pGraphicsFamily = i;
                        graphicsFamilyFound = true;
                }
                
                // Present Support
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentFamilyFound);
                if (presentFamilyFound)
                        *pPresentFamily = i;

                if (graphicsFamilyFound && presentFamilyFound)
                        break;
        }        
        
}

bool createDevice(VkPhysicalDevice physicalDevice, const uint32_t graphicsFamilyIndex, const uint32_t presentFamilyIndex, VkDevice *pDevice)
{
        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        // First the queues

        // vector holding all the queue Infos
        std::vector<VkDeviceQueueCreateInfo> queueInfos;

        float queuePriority = 1.0f;
        if (graphicsFamilyIndex == presentFamilyIndex)
        {
                VkDeviceQueueCreateInfo queueInfo = {};
                queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = graphicsFamilyIndex;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &queuePriority;
                queueInfos.push_back(queueInfo);   
        }
        else 
        {
                VkDeviceQueueCreateInfo graphicsQueueInfo = {};
                graphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                graphicsQueueInfo.queueFamilyIndex = graphicsFamilyIndex;
                graphicsQueueInfo.queueCount = 1;
                graphicsQueueInfo.pQueuePriorities = &queuePriority;

                VkDeviceQueueCreateInfo presentQueueInfo = {};
                presentQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                presentQueueInfo.queueFamilyIndex = presentFamilyIndex;
                presentQueueInfo.queueCount = 1;
                presentQueueInfo.pQueuePriorities = &queuePriority;

                queueInfos.push_back(graphicsQueueInfo);
                queueInfos.push_back(presentQueueInfo);
        }

        // tell the device creation struct
        deviceInfo.queueCreateInfoCount = queueInfos.size();
        deviceInfo.pQueueCreateInfos = queueInfos.data();



        // Device Features
        VkPhysicalDeviceVulkan13Features features13 = {};
        features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
        features13.dynamicRendering = VK_TRUE;
        features13.pNext = nullptr;

        // chain the struct to main device creation struct
        deviceInfo.pNext = &features13;



        // enable swapchain extension
        const std::vector<const char*> requiredDeviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        // Check if extension is present or not
        uint32_t deviceExtensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, nullptr);
        std::vector<VkExtensionProperties> availableDeviceExtensions(deviceExtensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &deviceExtensionCount, availableDeviceExtensions.data());

        for (int i=0; i<requiredDeviceExtensions.size(); ++i)
        {
                bool extensionFound = false;
                for (int j=0; j<availableDeviceExtensions.size(); ++j)
                {
                        if (std::strcmp(availableDeviceExtensions[j].extensionName, requiredDeviceExtensions[i]))  
                        {
                                extensionFound = true;
                                break;
                        }
                }
                if (!extensionFound)
                {
                        printf("Extensions: %s, not available in your GPU, exiting...\n", requiredDeviceExtensions[i]);\
                        return false;
                }
        }

        deviceInfo.enabledExtensionCount = requiredDeviceExtensions.size();
        deviceInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();


        // Disabling device features for now, optional to do since {} initializes it to nullptr anyways
        deviceInfo.pEnabledFeatures = nullptr;

        
        // creating the device
        if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, pDevice) != VK_SUCCESS) 
        {
                printf("failed to create logical device. exiting...");
                return false;
        }
        printf("Created logical device\n");
        return true;
}

void retrieveQueueIndexes(VkDevice device, const uint32_t graphicsFamilyIndex, const uint32_t presentFamilyIndex, VkQueue *pGraphicsQueue, VkQueue *pPresentQueue)
{
        vkGetDeviceQueue(device, graphicsFamilyIndex, 0, pGraphicsQueue);
        vkGetDeviceQueue(device, presentFamilyIndex, 0, pPresentQueue);
}