cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

#set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32/ /usr/local/x86_64-w64-mingw32/)

project(Graphics VERSION 0.0.2)

#add_subdirectory(glfw-linux)
set(SOURCES 
    ${PROJECT_SOURCE_DIR}/src/main.c 
    ${PROJECT_SOURCE_DIR}/src/glad.c
    ${PROJECT_SOURCE_DIR}/src/lib.c
    ${PROJECT_SOURCE_DIR}/src/shaders.c
    ${PROJECT_SOURCE_DIR}/include/glad/glad.h
    ${PROJECT_SOURCE_DIR}/include/KHR/khrplatform.h
    ${PROJECT_SOURCE_DIR}/include/shaders.h
    ${PROJECT_SOURCE_DIR}/include/lib.h
)
add_executable(Graphics ${SOURCES})

target_include_directories(Graphics PRIVATE
    glfw-3.3.8.bin.WIN64/include/
    src/
    include/
)
#add_library(GLFW SHARED IMPORTED)
#target_link_directories(GLFW PUBLIC 
#    "${PROJECT_SOURCE_DIR}/glfw-3.3.8.bin.WIN64/lib-mingw-w64"
#    )
target_link_libraries(Graphics 
    "${PROJECT_SOURCE_DIR}/glfw-3.3.8.bin.WIN64/lib-mingw-w64/libglfw3.a"
    "${PROJECT_SOURCE_DIR}/glfw-3.3.8.bin.WIN64/lib-mingw-w64/glfw3.dll"
    )
