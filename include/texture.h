#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shaders.h"

typedef struct __TEXTURE_STRUCT Texture;

Texture *tex_init(const char *image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);
void tex_unit(Shader *shader, const char *uniform, GLuint unit);
void tex_bind(Texture *texture);
void tex_unbind(Texture *texture);
void tex_del(Texture **texture);

#endif
