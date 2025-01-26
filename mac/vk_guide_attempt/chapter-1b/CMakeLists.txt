# cmake
cmake_minimum_required(VERSION 3.27)
project(chapter-1)
set(CMAKE_CXX_STANDARD 17)


add_executable(chapter-1b
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
include_directories(chapter-1b ${SDL2_INCLUDE_DIRS} shared)
target_link_libraries(chapter-1b glm::glm)
target_link_libraries(chapter-1b ${SDL2_LIBRARIES})
target_link_libraries(chapter-1b Vulkan::Vulkan)
target_link_libraries(chapter-1b fmt::fmt)

# Handled internally (git pull, stored in a folder ) 
target_link_libraries(chapter-1b vkguide_shared)
target_link_libraries(chapter-1b vkguide_third_party)