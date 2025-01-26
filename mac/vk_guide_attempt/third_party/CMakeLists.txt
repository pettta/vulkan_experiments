find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

add_library(vkguide_third_party INTERFACE)
add_library(stb_image INTERFACE)
add_library(vkbootstrap STATIC)

target_sources(vkbootstrap PRIVATE 
  vkbootstrap/VkBootstrap.h
  vkbootstrap/VkBootstrap.cpp
)
target_include_directories(vkbootstrap PUBLIC vkbootstrap)
target_link_libraries(vkbootstrap PUBLIC Vulkan::Vulkan $<$<BOOL:UNIX>:${CMAKE_DL_LIBS}>)
include_directories(vkguide_third_party ${SDL2_INCLUDE_DIRS} shared)



target_link_libraries(vkguide_third_party INTERFACE
glm::glm
${SDL2_LIBRARIES}
Vulkan::Vulkan
vkbootstrap
)


target_include_directories(vkguide_third_party INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)