#define GLFW_INCLUDE_VULKAN // Replaces  #include <Vulkan.h>, loading its own header 
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApplication {
private:  GLFWwindow* window; // holds pointer to GLFWwindow
private: VkInstance instance; // holds instance of connection between app and vulkan
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// Validation Layer setup - preprocessor directive 
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
// End Validation Layer preprocessor directive 


public:
    void run() {
        // GLFW Calls 
        initWindow(); 
        // Vulkan calls 
        initVulkan();
        mainLoop();
        cleanup();
    }

private:

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }
        return true;
    }

    void initWindow() {
        // Start glfw
        glfwInit();
        // Disable opengl context from init, Disable the complexities of resizing windows 
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);\
        // Create the window 
        window = glfwCreateWindow(WIDTH, HEIGHT, "window", nullptr, nullptr);
    }

    void initVulkan() {
        createInstance(); 
    }

    void createInstance(){
        // Optional Valdation layers logic that can be used for adding debugging 
        if (enableValidationLayers == true){
            std::cout << "MODE: DEBUG MODE" << std::endl;  
            if (!checkValidationLayerSupport()){
                throw std::runtime_error("validation layers requested, but not available!");
            }
        }else{
            std::cout << "MODE: PRODUCTION" << std::endl; 
        }

        
        // Optional struct that tells vulkan about our app 
        VkApplicationInfo appInfo{}; 
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Not optional struct that tells the Vulkan driver which global extensions and validation layers we want to use
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // specification of global extensions so vulkan can talk with glfw 
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        // Which global validation layers to enable? 
        createInfo.enabledLayerCount = 0;
        

        // Regarding Extensions -- Info about what vulkan supports(completely optional, CAN BE REMOVED SAFELY!!!)
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        std::cout << "available extensions:\n";
        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }


        // Regarding Extensions -- necessities for moltenvk to connect metal and vulkan (MACOS ONLY REQUIREMENT!!)
        std::vector<const char*> requiredExtensions;
        for(uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        // Create vulkan instance, check everything worked out 
        //VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

    }

    void mainLoop() {
        
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        // reverse-order of creation 
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}