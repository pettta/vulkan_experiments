cmake_minimum_required(VERSION 3.27)
project(chapter-0)
set(CMAKE_CXX_STANDARD 17)

add_executable(chapter-0 
  main.cpp
  vk_engine.cpp
  vk_engine.h
  vk_types.h
  vk_mem_alloc.h
  vk_initializers.cpp
  vk_initializers.h
  )

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
include_directories(chapter-0 ${SDL2_INCLUDE_DIRS})


target_link_libraries(chapter-0 glm::glm)
target_link_libraries(chapter-0 ${SDL2_LIBRARIES})
target_link_libraries(chapter-0 Vulkan::Vulkan)