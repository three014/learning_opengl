#include "lib.h"
#include "shaders.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MAX_TIME_STRLEN 1024

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glad_glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

/// @brief Prints current time and error messages to the console.
/// @param err The short-form of the error message.
/// @param err_str The more expanded form of the error message.
/// @return 0 for "BAD"
unsigned int error_callback(char *err, char *err_str) {
    char buffer[MAX_TIME_STRLEN];
    
#ifdef _WIN32
    __time64_t t;
    _time64(&t);
    struct tm the_time;
    int guwah = localtime_s(&the_time, &t);
    unsigned int display_time = (guwah == 0) && strftime(buffer, sizeof(buffer), "%a %c", &the_time);
#else
    time_t t = time(0);
    struct tm *the_time = localtime(&t);

    unsigned int display_time = the_time && strftime(buffer, sizeof(buffer), "%a %m/%d/%Y %T", the_time);
#endif

    if (display_time && err && err_str) {
        printf("%s ERROR::%s %s\n", buffer, err, err_str);
    }
    else if (!display_time && err && err_str) {
        printf("[Unknown Time] ERROR::%s %s\n", err, err_str);
    }
    else if (display_time && err && !err_str) {
        printf("%s ERROR::%s\n", buffer, err);
    }
    else {
        printf("[Unknown Time] ERROR::UNKNOWN\n");
    }

    return BAD;
}


/// @brief Prints current time and info message to console. 
/// @param info The message to be displayed to the console.
/// @return 1 for "OK"
unsigned int info_callback(char *info) {
    char buffer[MAX_TIME_STRLEN];

#ifdef _WIN32
    __time64_t t;
    _time64(&t);
    struct tm the_time;
    int guwah = localtime_s(&the_time, &t);
    unsigned int display_time = (guwah == 0) && strftime(buffer, sizeof(buffer), "%a %c", &the_time);
#else
    time_t t = time(0);
    struct tm *the_time = localtime(&t);

    unsigned int display_time = the_time && strftime(buffer, sizeof(buffer), "%a %m/%d/%Y %T", the_time);
#endif

    if (display_time && info) {
        printf("%s INFO::%s\n", buffer, info);
    }
    else if (!display_time && info) {
        printf("[Unknown Time] INFO::%s\n", info);
    }
    else if (display_time && !info) {
        printf("%s INFO::OK\n", buffer);
    }
    else {
        printf("[Unknown Time] INFO::OK\n");
    }

    return OK;
}

