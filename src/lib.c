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

void build_vertex_shader(FILE *file_in,
                         char **vs_str,
                         unsigned int max_strlen,
                         unsigned int* vertex_shader)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    int parse_correctly = parse_shader(file_in, vs_str, max_strlen);
    if (parse_correctly)
    {
        *vertex_shader = glad_glCreateShader(GL_VERTEX_SHADER);
        glad_glShaderSource(*vertex_shader, 1,
                (const char *const *) vs_str, NULL);

        glad_glCompileShader(*vertex_shader);
        
        glad_glGetShaderiv(*vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glad_glGetShaderInfoLog(*vertex_shader, INFO_LOG_SIZE, NULL, info_log);
            error_callback("SHADER::VERTEX::COMPILATION_FAILED", info_log);
        }
    }
}

void build_fragment_shader(FILE *file_in, 
                           char **fs_str, 
                           unsigned int max_strlen, 
                           unsigned int* fragment_shader)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    int parse_correctly = parse_shader(file_in, fs_str, max_strlen);    
    if (parse_correctly)
    {
        *fragment_shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
        glad_glShaderSource(*fragment_shader, 1, 
                (const char *const *) fs_str, NULL);
        glad_glCompileShader(*fragment_shader);

        glad_glGetShaderiv(*fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glad_glGetShaderInfoLog(*fragment_shader, 
                    INFO_LOG_SIZE, NULL, info_log);
            error_callback(
                    "SHADER::FRAGMENT::COMPILATION_FAILED", 
                    info_log);
        }
    }
}

int compile_shaders(unsigned int *vertex_shader,
                    unsigned int *fragment_shader,
                    unsigned int *shader_program)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    *shader_program = glad_glCreateProgram();
    glad_glAttachShader(*shader_program, *vertex_shader);
    glad_glAttachShader(*shader_program, *fragment_shader);
    glad_glLinkProgram(*shader_program);

    glad_glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glad_glGetProgramInfoLog(*shader_program, 512, NULL, info_log);
        error_callback("SHADER::PROGRAM::LINKING_FAILED", info_log);
        return 0;
    }

    return 1;
}
