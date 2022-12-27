#ifndef __EBO_H
#define __EBO_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct __EBO_STRUCT EBO;

EBO *ebo_init(GLuint *vertices, GLsizeiptr size);
void ebo_bind(EBO *self);
void ebo_unbind();
void ebo_del(EBO **del);

#endif
