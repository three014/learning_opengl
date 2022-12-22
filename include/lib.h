#ifndef __LIB_H
#define __LIB_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <errno.h>

#define INFO_LOG_SIZE 512
#define BAD 0
#define OK 1
#define TRUE 1
#define FALSE 0

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *, int, int);
unsigned int error_callback(char *err, char *err_str);
unsigned int info_callback(char *info);

#endif
