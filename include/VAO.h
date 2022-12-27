#ifndef __VAO_H
#define __VAO_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "VBO.h"

typedef struct __VAO_STRUCT VAO;

VAO *vao_init();
void vao_linkattrib(VAO *vao, VBO *vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void *offset);
void vao_bind(VAO *self);
void vao_unbind();
void vao_del(VAO **del);

#endif
