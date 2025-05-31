# cmake
cmake_minimum_required(VERSION 3.27)
project(chapter-1b)
set(CMAKE_CXX_STANDARD 17)
if(APPLE)
    add_compile_options(-Wno-nullability-completeness)
endif()
list(APPEND CMAKE_PREFIX_PATH 
    "~/VulkanSDK/1.4.304.1/macOS"
    "/opt/homebrew"
)


add_executable(chapter-2
  main.cpp
  vk_engine.cpp
  vk_engine.h
)
add_subdirectory("../shared" "${CMAKE_CURRENT_BINARY_DIR}/shared")
add_subdirectory("../third_party" "${CMAKE_CURRENT_BINARY_DIR}/third_party")

# Handled by package managers (brew on mac) 
find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(fmt REQUIRED)
find_package(volk REQUIRED)

include_directories(chapter-2 ${SDL2_INCLUDE_DIRS} shared)
include_directories(chapter-2 ${Vulkan_INCLUDE_DIRS} shared)
target_link_libraries(chapter-2
  glm::glm
  ${SDL2_LIBRARIES}
  Vulkan::Vulkan
  fmt::fmt 
  volk::volk volk::volk_headers
  vkguide_shared
  vkguide_third_party
  )