find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

add_library(vkguide_third_party INTERFACE)
add_library(stb_image INTERFACE)
add_library(vkbootstrap STATIC)
add_library(imgui STATIC)

## Vk bootstrap 
target_sources(vkbootstrap PRIVATE 
  vkbootstrap/VkBootstrap.h
  vkbootstrap/VkBootstrap.cpp
)
target_include_directories(vkbootstrap PUBLIC vkbootstrap)
target_link_libraries(vkbootstrap PUBLIC Vulkan::Vulkan $<$<BOOL:UNIX>:${CMAKE_DL_LIBS}>)

## Imgui
target_sources(imgui PRIVATE 
  imgui/imgui.h
  imgui/imgui.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_tables.cpp
  imgui/backends/imgui_impl_vulkan.cpp
  imgui/backends/imgui_impl_sdl2.cpp
)
target_include_directories(imgui PUBLIC 
  imgui
  imgui/backends
)
target_link_libraries(imgui PUBLIC
  Vulkan::Vulkan
  ${SDL2_LIBRARIES}
)


include_directories(vkguide_third_party ${SDL2_INCLUDE_DIRS} shared)



target_link_libraries(vkguide_third_party INTERFACE
glm::glm
${SDL2_LIBRARIES}
Vulkan::Vulkan
vkbootstrap
imgui
)


target_include_directories(vkguide_third_party INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)