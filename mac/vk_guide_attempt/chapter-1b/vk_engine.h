// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.
//> intro
#pragma once

#include "vk_types.h"

class VulkanEngine {
public:
	// Store Vulkan fundamental objects 
	VkInstance _instance;// Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger;// Vulkan debug output handle
	VkPhysicalDevice _chosenGPU;// GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands
	VkSurfaceKHR _surface;// Vulkan window surface

	// Store swapchain fundamental objects
	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;

	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;
	VkExtent2D _swapchainExtent;

	// Store window related fundamentals 
	bool _isInitialized{ false };
	int _frameNumber {0};
	bool stop_rendering{ false };
	VkExtent2D _windowExtent{ 1700 , 900 };
	struct SDL_Window* _window{ nullptr };
	static VulkanEngine& Get();

	
	void init(); //initializes everything in the engine
	void cleanup(); //shuts down the engine
	void draw(); //draw loop
	void run(); //run main loop

private:
	void init_vulkan();

	void init_swapchain();
	void create_swapchain(uint32_t width, uint32_t height);
	void destroy_swapchain();

	void init_commands();
	void init_sync_structures();

};
//< intro