#include "lib.h"
#include "shaders.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MAX_TIME_STRLEN 1024

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glad_glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void error_callback(char *err, char *err_str)
{
    char buffer[MAX_TIME_STRLEN];
    
#ifdef _WIN32
    __time64_t t;
    _time64(&t);
    struct tm the_time;
    int guwah = localtime_s(&the_time, &t);
    unsigned int display_time = (guwah == 0)
            && strftime(buffer, sizeof(buffer), "%a %c", &the_time);
#else
    time_t t = time(0);
    struct tm *the_time = localtime(&t);

    unsigned int display_time = the_time 
            && strftime(buffer, sizeof(buffer), "%a %m/%d/%Y %T", the_time);
#endif

    if (display_time)
    {
        printf("%s ERROR::%s %s\n", buffer, err, err_str);
    }
    else
    {
        printf("[Unknown Time] ERROR::%s, %s\n", err, err_str);
    }
}

void info_callback(char *info)
{
    char buffer[MAX_TIME_STRLEN];

#ifdef _WIN32
    __time64_t t;
    _time64(&t);
    struct tm the_time;
    int guwah = localtime_s(&the_time, &t);
    unsigned int display_time = (guwah == 0)
            && strftime(buffer, sizeof(buffer), "%a %c", &the_time);
#else
    time_t t = time(0);
    struct tm *the_time = localtime(&t);

    unsigned int display_time = the_time 
            && strftime(buffer, sizeof(buffer), "%a %m/%d/%Y %T", the_time);
#endif

    if (display_time)
    {
        printf("%s INFO::%s\n", buffer, info);
    }
    else 
    {
        printf("[Unknown Time] INFO::%s\n", info);
    }
}

