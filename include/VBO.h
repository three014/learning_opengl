#ifndef __VBO_H
#define __VBO_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct __VBO_STRUCT VBO;

VBO *vbo_init(GLfloat *vertices, GLsizeiptr size);
void vbo_bind(VBO *self);
void vbo_unbind();
void vbo_del(VBO **del);

#endif
