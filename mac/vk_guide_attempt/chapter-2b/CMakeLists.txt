# cmake
cmake_minimum_required(VERSION 3.27)
project(chapter-2b)
# using c++20 as it uses std::span in shared/vk_descriptors 
set(CMAKE_CXX_STANDARD 20)
if(APPLE)
    add_compile_options(-Wno-nullability-completeness)
endif()
list(APPEND CMAKE_PREFIX_PATH 
    "~/VulkanSDK/1.4.304.1/macOS"
    "/opt/homebrew"
)


add_executable(chapter-2b
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

include_directories(chapter-2b ${SDL2_INCLUDE_DIRS} shared)
include_directories(chapter-2b ${Vulkan_INCLUDE_DIRS} shared)
target_link_libraries(chapter-2b
  glm::glm
  ${SDL2_LIBRARIES}
  Vulkan::Vulkan
  fmt::fmt 
  volk::volk volk::volk_headers
  vkguide_shared
  vkguide_third_party
  )


# Deal with shaders --> turn to SPIR-V binaries 
# TODO NOT SURE IF THIS WORKS YET, IF END OF CHAPTER STILL BUGS TRY HERE 
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/shaders/*.comp"
)

# Create binary output directory in build folder
foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders/"
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL}
  )
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)
add_dependencies(chapter-2b Shaders)