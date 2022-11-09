#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <string.h>

#define MAX_SHADER_SIZE 256 * 10
#define BUFFER_SIZE 256
#define NULL_CHAR '\0'

typedef struct __VERTEX_SHADER 
{
    unsigned int id;
    char file_loc[BUFFER_SIZE];
    char *src_code;
} vertex_shader;

typedef struct __FRAGMENT_SHADER
{
    unsigned int id;
    char file_loc[BUFFER_SIZE];
    char *src_code;
} fragment_shader;

typedef struct __VERTEX_FRAGMENT_SHADER_PROGRAM
{
    unsigned int program;
    vertex_shader vert;
    fragment_shader frag;
} basic_shader;

int parse_shader(FILE *, char **);
void build_vertex_shader(vertex_shader *);
void build_fragment_shader(fragment_shader *);
int compile_shaders(unsigned int *, unsigned int *, unsigned int *);

#endif
