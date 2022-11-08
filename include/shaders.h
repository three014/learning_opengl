#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256
#define NULL_CHAR '\0'

int parse_shader(FILE *, char **, int);

#endif
