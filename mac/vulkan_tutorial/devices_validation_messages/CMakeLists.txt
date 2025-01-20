cmake_minimum_required(VERSION 3.27)
project(devices_validation_messages)

set(CMAKE_CXX_STANDARD 17)

# Set build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")


add_executable(devices_validation_messages main.cpp)

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(devices_validation_messages glm::glm)
target_link_libraries(devices_validation_messages glfw)
target_link_libraries(devices_validation_messages Vulkan::Vulkan)

# Set different compile flags for Debug and Release
# Set different compile flags and definitions for Debug and Release
target_compile_options(devices_validation_messages PRIVATE
    $<$<CONFIG:Debug>:${CMAKE_CXX_FLAGS_DEBUG}>
)

target_compile_definitions(devices_validation_messages PRIVATE
    $<$<CONFIG:Release>:NDEBUG>
)

# Custom targets for easy building
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} --build . --config Debug
    COMMENT "Building debug version")

add_custom_target(release
    COMMAND ${CMAKE_COMMAND} --build . --config Release
    COMMENT "Building release version")