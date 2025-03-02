# download the vulkan installer for mac, then run the installer via 
# sudo ./InstallVulkan.app/Contents/MacOS/InstallVulkan --root ~/VulkanSDK/1.4.304.1 --accept-licenses --default-answer --confirm-command install com.lunarg.vulkan.core com.lunarg.vulkan.usr com.lunarg.vulkan.sdl2 com.lunarg.vulkan.glm com.lunarg.vulkan.volk com.lunarg.vulkan.vma com.lunarg.vulkan.ios
# test with vkvia, and vulkaninfo
export VULKAN_SDK=/Users/thomaspett/VulkanSDK/1.4.304.1/macOS
export PATH=$VULKAN_SDK/bin:$PATH
export DYLD_LIBRARY_PATH=$VULKAN_SDK/lib:$DYLD_LIBRARY_PATH
export VK_ICD_FILENAMES=$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$VULKAN_SDK/share/vulkan/explicit_layer.d
