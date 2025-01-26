//> includes 
// -- Stdlb -- //
#include <chrono>
#include <thread>

// -- Inside folder -- // 
#include "vk_engine.h"


// -- Extrenal 

#include <SDL.h>
#include <SDL_vulkan.h>

#include "vk_initializers.h"
#include "vk_types.h"

#include "VkBootstrap.h"
//< includes

//> init
constexpr bool bUseValidationLayers = false;

VulkanEngine* loadedEngine = nullptr;

VulkanEngine& VulkanEngine::Get() { return *loadedEngine; }
void VulkanEngine::init()
{
    // Init SDL, create window 
    SDL_Init(SDL_INIT_VIDEO);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);
    _window = SDL_CreateWindow(
        "Vulkan Engine",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        _windowExtent.width,
        _windowExtent.height,
        window_flags);

    // Build out app 
    init_vulkan(); 
    init_swapchain(); 
    init_commands(); 
    init_sync_structures();

    // everything went fine
    _isInitialized = true;
}

void VulkanEngine::init_vulkan()
{
    /** Use vkbootstrap to create a Vulkan instance **/
    
    //make the vulkan instance, with basic debug features
    vkb::InstanceBuilder builder;
	auto inst_ret = builder.set_app_name("Example Vulkan Application")
		.request_validation_layers(bUseValidationLayers)
		.use_default_debug_messenger()
		.require_api_version(1, 2, 0) // 1.2 rather than 1.3 on mac 
		.build();
	vkb::Instance vkb_inst = inst_ret.value();

	//grab the instance & debug messenger, need to store so we can prevent leaks 
	_instance = vkb_inst.instance;
	_debug_messenger = vkb_inst.debug_messenger;

    /** Use vkbootstrap to create a surface and chose what device to use **/
    // (MAC MOMENT! - This is the part that is different from the reference code since mac doesnt support 1.3 yet)
    
    SDL_Vulkan_CreateSurface(_window, _instance, &_surface);

	//vulkan 1.3 features(Not supported on mac)
	// VkPhysicalDeviceVulkan13Features features{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	// features.dynamicRendering = true;
	// features.synchronization2 = true;

	//vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;
    VkPhysicalDeviceSynchronization2Features sync2Features{};
    sync2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
    sync2Features.synchronization2 = true;
	//use vkbootstrap to select a gpu. 
	//We want a gpu that can write to the SDL surface and supports vulkan 1.3 (but alas we are on mac, so add the same features manually)
	vkb::PhysicalDeviceSelector selector{ vkb_inst };
	vkb::PhysicalDevice physicalDevice = selector
		.set_minimum_version(1, 2)
		.set_required_features_12(features12)
        .add_required_extension(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME)
        .add_required_extension(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME)
        .add_required_extension(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
        .add_required_extension(VK_KHR_MULTIVIEW_EXTENSION_NAME)
        .add_required_extension(VK_KHR_MAINTENANCE_2_EXTENSION_NAME)
        .add_required_extension(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME)
        .add_required_extension_features(sync2Features)
		.set_surface(_surface)
		.select()
		.value();

	// //create the final vulkan device
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };
	vkb::Device vkbDevice = deviceBuilder.build().value();

	// // Get the VkDevice handle used in the rest of a vulkan application
	_device = vkbDevice.device;
	_chosenGPU = physicalDevice.physical_device;

    /**Use VKbootstrap to init graphics queue and family  */
    // NOTE: these lines below might be causing issues 
    _graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value(); 
    _graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
}
void VulkanEngine::create_swapchain(uint32_t width, uint32_t height)
{
	vkb::SwapchainBuilder swapchainBuilder{ _chosenGPU,_device,_surface };

	_swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM; // Colorspace 
	vkb::Swapchain vkbSwapchain = swapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = _swapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build()
		.value();

	_swapchainExtent = vkbSwapchain.extent;
	//store swapchain and its related images
	_swapchain = vkbSwapchain.swapchain;
	_swapchainImages = vkbSwapchain.get_images().value();
	_swapchainImageViews = vkbSwapchain.get_image_views().value();
}

void VulkanEngine::init_swapchain()
{
	create_swapchain(_windowExtent.width, _windowExtent.height);
}

/**We will use VK init for the next two, but its basically 
Create info struct --> fill attributes --> check everything works out **/

void VulkanEngine::init_commands()
{
    // command pool info --> command buffer info --> verify
    VkCommandPoolCreateInfo commandPoolInfo = vkinit::command_pool_create_info(_graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    for (int i = 0; i < FRAME_OVERLAP; i++) {
        // NOTE: this line below might be causing issues 
		VK_CHECK(vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &_frames[i]._commandPool));
		// allocate the default command buffer that we will use for rendering
		VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_frames[i]._commandPool, 1);
		VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_frames[i]._mainCommandBuffer));
	}
}
void VulkanEngine::init_sync_structures()
{
    //fenceinfo --> sempaphore info --> verify 
	//1 fence to control gpu when finished render frame, 2 semaphores to sync render with swapchain
	//we want the fence to start signalled so we can wait on it on the first frame (VK_FENCE_CREATE_SIGNALED_BIT)
	VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);
	VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();
	for (int i = 0; i < FRAME_OVERLAP; i++) {
		VK_CHECK(vkCreateFence(_device, &fenceCreateInfo, nullptr, &_frames[i]._renderFence));
		VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._swapchainSemaphore));
		VK_CHECK(vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_frames[i]._renderSemaphore));
	}
}
//< init

//> extras
void VulkanEngine::destroy_swapchain()
{
	vkDestroySwapchainKHR(_device, _swapchain, nullptr);

	// destroy swapchain resources
	for (int i = 0; i < _swapchainImageViews.size(); i++) {

		vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
	}
}

void VulkanEngine::cleanup()
{
    // Cleanup in reverse order (throws errors otherwise due to leaks)
    // Commands --> Sync --> Swapchain --> Surface --> Device --> Debug --> Instance --> Window
    if (_isInitialized) {
		vkDeviceWaitIdle(_device);
		for (int i = 0; i < FRAME_OVERLAP; i++) {
			vkDestroyCommandPool(_device, _frames[i]._commandPool, nullptr);
        }
		destroy_swapchain();
		vkDestroySurfaceKHR(_instance, _surface, nullptr);
		vkDestroyDevice(_device, nullptr);
		vkb::destroy_debug_utils_messenger(_instance, _debug_messenger);
		vkDestroyInstance(_instance, nullptr);
		SDL_DestroyWindow(_window);
	}
}

void VulkanEngine::draw()
{
//> draw_1: wait until the gpu has finished rendering the last frame. Timeout of 1s
	VK_CHECK(vkWaitForFences(_device, 1, &get_current_frame()._renderFence, true, 1000000000));
	VK_CHECK(vkResetFences(_device, 1, &get_current_frame()._renderFence));
//< draw_1

//> draw_2: request image from the swapchain
	uint32_t swapchainImageIndex;
	VK_CHECK(vkAcquireNextImageKHR(_device, _swapchain, 1000000000, get_current_frame()._swapchainSemaphore, nullptr, &swapchainImageIndex));
//< draw_2

//> draw_3: begin recording the command buffer
	VkCommandBuffer cmd = get_current_frame()._mainCommandBuffer;

	// now that the cmds finished executing, we can safely reset the cmd buffer to begin recording again.
	VK_CHECK(vkResetCommandBuffer(cmd, 0));

	//begin the cmd buffer recording. We use this cmd buffer once, so we let vulkan know that
	VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));
//< draw_3

}
//< extras

//> drawloop
void VulkanEngine::run()
{
    SDL_Event e;
    bool bQuit = false;

    // main loop
    while (!bQuit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // close the window when user alt-f4s or clicks the X button
            if (e.type == SDL_QUIT)
                bQuit = true;

            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
                    stop_rendering = true;
                }
                if (e.window.event == SDL_WINDOWEVENT_RESTORED) {
                    stop_rendering = false;
                }
            }
        }

        // do not draw if we are minimized
        if (stop_rendering) {
            // throttle the speed to avoid the endless spinning
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        draw();
    }
}
//< drawloop
