#ifndef LIB_H
#define LIB_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>

#define INFO_LOG_SIZE 512

void processInput(GLFWwindow *);
void framebuffer_size_callback(GLFWwindow *, int, int);
void error_callback(char *, char *);
void info_callback(char *);

#endif
