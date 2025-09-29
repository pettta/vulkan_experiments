# Tried to mess around with https://vkguide.dev/ 

- Doesn't play super nicefully with mac if you aren't already super familiar with vulkan
- Comes with a bunch of files, but hard to debug the cmake issues imo with so many files just to start 
- Tried to get it to work for about 5 hours than gave up and went to a different tutorial, will come back to it later 

# Then I went to try out https://vulkan-tutorial.com/
- Uses GLFW rather than SDL for the window 
- much longer and more explained beginnings but doesn't get as far 
- about halfway through the second chapter rotated back to the first attempt and got the basic window working that way 

# Requirements 
## Requirements For Mac (through homebrew)
- clang++, cmake 
- VulkanSDK==1.4.304.0
- Volk==1.4.304.1
- SDL== 
## Requirements for windows (through visual studio installer)
- Install MSVC C/C++ Compiler `cl` 
- Install Cmake and Nmake 
- Install windows 10 SDK 
- we have everything else stored in files in the repo for windows 

# Building 
## Building on mac 
- `cmake -B build .` 
- `cd ./build`
- `make`
## Building on windows 
- `cmake -S . -B build -G "Visual Studio 16 2019" -A x64` 
- `cd ./build`
- `nmake` 