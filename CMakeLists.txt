cmake_minimum_required(VERSION 3.16.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
#set(THREADS_PREFER_PTHREAD_FLAG ON)

#set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32/ /usr/local/x86_64-w64-mingw32/)

project(Graphics VERSION 0.0.2)

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/src/main.c 
    ${PROJECT_SOURCE_DIR}/src/glad.c
    ${PROJECT_SOURCE_DIR}/src/VBO.c
    ${PROJECT_SOURCE_DIR}/src/VAO.c
    ${PROJECT_SOURCE_DIR}/src/EBO.c
    ${PROJECT_SOURCE_DIR}/src/texture.c
    ${PROJECT_SOURCE_DIR}/include/texture.h
    ${PROJECT_SOURCE_DIR}/include/VBO.h
    ${PROJECT_SOURCE_DIR}/include/VAO.h
    ${PROJECT_SOURCE_DIR}/include/EBO.h
    ${PROJECT_SOURCE_DIR}/src/stb.c
    ${PROJECT_SOURCE_DIR}/src/lib.c
    ${PROJECT_SOURCE_DIR}/src/shaders.c
    ${PROJECT_SOURCE_DIR}/include/glad/glad.h
    ${PROJECT_SOURCE_DIR}/include/KHR/khrplatform.h
    ${PROJECT_SOURCE_DIR}/include/shaders.h
    ${PROJECT_SOURCE_DIR}/include/lib.h
    ${PROJECT_SOURCE_DIR}/include/stb/stb_image.h
)

add_executable(Graphics ${SOURCES})

target_include_directories(Graphics PUBLIC
    glfw-3.3.8.bin.WIN64/include/
    src/
    include/
)

target_link_libraries(Graphics PRIVATE
    #"${PROJECT_SOURCE_DIR}/libraries/libwinpthread-1.dll"
    "${PROJECT_SOURCE_DIR}/glfw-3.3.8.bin.WIN64/lib-mingw-w64/libglfw3.a"
    "${PROJECT_SOURCE_DIR}/glfw-3.3.8.bin.WIN64/lib-mingw-w64/glfw3.dll"
)
