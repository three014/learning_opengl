#ifndef __SHADERS_H
#define __SHADERS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <string.h>


typedef struct __VERTEX_FRAGMENT_SHADER_PROGRAM Shader;


int sh_prog_init(const char *vertex_file, const char *fragment_file, Shader **out);
void sh_prog_del(Shader **del);
void sh_activate(Shader *shader_program);
GLint sh_get_uniloc(Shader *shader_program, const char *var_name);

#endif
