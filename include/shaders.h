#ifndef __SHADERS_H
#define __SHADERS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct __VERTEX_FRAGMENT_SHADER_PROGRAM Shader;

unsigned int sh_prog_init(const char *vertex_file, const char *fragment_file, Shader **out);
void sh_prog_del(Shader **del);
void sh_activate(Shader *shader_program);
unsigned int sh_get_uniloc(Shader *shader_program, const char *var_name, GLint *uniloc);

#endif
