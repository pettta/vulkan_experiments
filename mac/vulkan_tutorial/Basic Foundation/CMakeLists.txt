cmake_minimum_required(VERSION 3.27)
project(vulkanTutorial)

set(CMAKE_CXX_STANDARD 17)

add_executable(vulkanTutorial main.cpp)

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(vulkanTutorial glm::glm)
target_link_libraries(vulkanTutorial glfw)
target_link_libraries(vulkanTutorial Vulkan::Vulkan)