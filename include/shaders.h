#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <string.h>

#define MAX_SHADER_SIZE 256 * 10
#define BUFFER_SIZE 256
#define NULL_CHAR '\0'

#define VERT 0 
#define FRAG 1
#define NUM_SMALL_SHADERS 2

typedef struct __VERTEX_OR_FRAGMENT_SHADER
{
    GLuint ID;
    GLenum type;
    char *src_code;
} small_shader;

typedef struct __VERTEX_FRAGMENT_SHADER_PROGRAM
{
    GLuint ID;
    small_shader shaders[NUM_SMALL_SHADERS];
} Shader;

char *parse_file(const char *src);
int compile_shader(Shader *program, int type);
int compile_program(Shader **program);
int create_shader_prog(const char *vertex_file, const char *fragment_file, Shader **out);
void delete_shader_prog(Shader *del);

#endif
