# cmake
cmake_minimum_required(VERSION 3.27)
project(chapter-0b)
set(CMAKE_CXX_STANDARD 17)


add_executable(chapter-0b
  main.cpp
  vk_engine.cpp
  vk_engine.h
)
add_subdirectory("../shared" "${CMAKE_CURRENT_BINARY_DIR}/shared")
add_subdirectory("../third_party" "${CMAKE_CURRENT_BINARY_DIR}/third_party")

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(chapter-0b ${SDL2_INCLUDE_DIRS} shared)
target_link_libraries(chapter-0b glm::glm)
target_link_libraries(chapter-0b ${SDL2_LIBRARIES})
target_link_libraries(chapter-0b Vulkan::Vulkan)
target_link_libraries(chapter-0b vkguide_shared)
target_link_libraries(chapter-0b vkguide_third_party)