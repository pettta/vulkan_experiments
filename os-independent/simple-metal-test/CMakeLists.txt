cmake_minimum_required(VERSION 3.20)

project(simple_metal_test LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

# Place build artefacts in a shared bin folder alongside the source tree
set(_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${_OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${_OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${_OUTPUT_DIR}")

if(CMAKE_CONFIGURATION_TYPES)
    foreach(_CONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER "${_CONFIG}" _CONFIG_UPPER)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_CONFIG_UPPER} "${_OUTPUT_DIR}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_CONFIG_UPPER} "${_OUTPUT_DIR}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_CONFIG_UPPER} "${_OUTPUT_DIR}")
    endforeach()
endif()

add_subdirectory(third_party)

add_executable(example_sdl2_metal
    metal-engine/metal_engine.mm
)

target_link_libraries(example_sdl2_metal PRIVATE
    imgui
)

if(TARGET SDL2::SDL2)
    target_link_libraries(example_sdl2_metal PRIVATE SDL2::SDL2)
else()
    target_include_directories(example_sdl2_metal PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(example_sdl2_metal PRIVATE ${SDL2_LIBRARIES})
endif()

target_link_libraries(example_sdl2_metal PRIVATE
    "-framework Metal"
    "-framework MetalKit"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework QuartzCore"
)

target_compile_options(example_sdl2_metal PRIVATE
    -Wall
    -Wformat
    $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-weak>
    $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>
)