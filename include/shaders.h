#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 256
#define NULL_CHAR '\0'

int parse_shader(FILE *, char **, int);
void build_vertex_shader(FILE *, char **, unsigned int, unsigned int *);
void build_fragment_shader(FILE *, char **, unsigned int, unsigned int *);
int compile_shaders(unsigned int *, unsigned int *, unsigned int *);

#endif
