// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.
//> intro
#pragma once

#include "vk_types.h"



struct DeletionQueue 
{
	// std::functions are rather large for everything 
	// an improvement here would be to have arrays for types like vkimage, vkbuffer, etc.
	// then loop through and delete them 
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		// reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)(); //call functors
		}

		deletors.clear();
	}
};

struct FrameData {
	// We need to GPUsync without race conditions  
	VkSemaphore _swapchainSemaphore, _renderSemaphore; // Sync between GPU and GPU 
	VkFence _renderFence; // Sync between Device (GPU) and Host (CPU)


	// For vulkan commands we need to store the command pools and buffers 
	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;

	// Deletion queue for vulkan objects 
	DeletionQueue _deletionQueue; 
};

//double buffering 
// TODO IN FUTURE: Implement triple buffering, V-sync, and frame limiting 
constexpr unsigned int FRAME_OVERLAP = 2; 


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

	// Store objects(queues) for commands 
	FrameData _frames[FRAME_OVERLAP];
	FrameData& get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; };
	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

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

	// Creation and Cleanup for vulkan objects 
	DeletionQueue _mainDeletionQueue; 
	VmaAllocator _allocator; 

	
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