# Basic Graphics Project for Windows

I'm building for Windows but developing on WSL (Ubuntu 20.04.5 LTS)

While I am doing this project to learn how to create graphical programs using OpenGL, this is also for me to develop clean and understandable C code. 

## Dependencies for this project:
- glfw 3.3.8 for Windows (64-bit pre-compiled binaries)
- MinGW-w64 - gcc and g++
- cmake 3.16.3 or higher
- Glad OpenGL Loader (Core, Version 3.3) 
  - Should be included already in include/ and src/
- (I'll add more to this list as I remember)

## Steps to get up and running:
1. Install MinGW-w64 and its dependencies
2. Clone repository
3. Download glfw binaries and place in project folder
4. In the main project folder run `cmake -S . -B build`
5. `cd build`
6. Run `cmake --build .`
7. Start program with ./Graphics.exe in command-line, or double-click Graphics.exe in a file explorer.
