# Progress Diary 
## Tried to mess around with https://vkguide.dev/ 

- Doesn't play super nicefully with mac if you aren't already super familiar with vulkan
- Comes with a bunch of files, but hard to debug the cmake issues imo with so many files just to start 
- Tried to get it to work for about 5 hours than gave up and went to a different tutorial, will come back to it later 

## Then I went to try out https://vulkan-tutorial.com/
- Uses GLFW rather than SDL for the window 
- much longer and more explained beginnings but doesn't get as far 
- about halfway through the second chapter rotated back to the first attempt and got the basic window working that way 

# Requirements 
https://vulkan.lunarg.com/sdk/home for vulkanSDK (to test everything worked run `vkcube` to see a spinning cube)

## Requirements For Mac (through homebrew)
- VulkanSDK==1.4.304.0 (get from website)
- clang++, cmake
- Volk==1.4.304.1
- SDL== 
## Requirements for windows (through visual studio installer)
https://visualstudio.microsoft.com/vs/community/ 
- Install MSVC C/C++ Compiler `cl` 
- Install Cmake>=3.21 and Nmake 
- Install windows 10 SDK 
- VulkanSDK==1.4.321.1 (get from webite)
- we have everything else stored in files in the repo for windows 

# Building 
## Manual Building 
### Building on mac in the mac repo
- `cmake -B build .` 
- `cd ./build`
- `make`
### Building on windows in the windows repo
- `cmake -B build .` 
- `cmake --build build --config Release`
- `cd ./bin`
- `chapter_0.exe` 
## Os-independent automated building
- `python local_setup.py` 

# Long Term Goals:
- spir-v shader hot reloading at runtime. 
    have an imgui window that has two sections: 
    (1) on the left is the shader source code 
    (2) on the right is the output of the shader rendered assuming it throws no errors, else it displays a message 
    (3) export functionality of images and videos produced with the shader section  

- OS independent building and running 
    ideally the user on linux, mac, windows just runs some script at the top level and it builds everything they need to 
    then they just run another script to run it. This should also just use a single python one-liner. 

- really modular code so that someone can drop this into a new repo, then start working from it as a base tool
    i imagine say someone wants to make some IMGUI based program that also has a shader hot reloader in it to test shaders alongside the other
    plugings that they are making

- Optional build mode for "Debug" which not only logs errors and stuff in the console 
    but also launches a tracy server advises you to run the executable so you can do analysis of performance 
