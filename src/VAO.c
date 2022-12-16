#include "VAO.h"
#include "lib.h"

#include <stdlib.h>
#include <string.h>

typedef struct __VAO_STRUCT
{
    GLuint ID;
} VAO;


VAO *vao_init()
{
    VAO *vao = malloc(sizeof *vao);
    if (vao == NULL)
    {
        error_callback("VBO::INIT::ALLOCATION_FAILED", strerror(errno));
        return 0;
    }

    glGenVertexArrays(1, &(vao->ID));

    return vao;
}

void vao_linkattrib(VAO *vao, VBO *vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void *offset)
{
    vao_bind(vao);
    vbo_bind(vbo);
    glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);

    // Enable the vertex attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(layout);
    vbo_unbind();
    
}

void vao_bind(VAO *self)
{
    if (self == NULL)
    {
        return;
    }
    glBindVertexArray(self->ID);
}

void vao_unbind()
{
    glBindVertexArray(0);
}

void vao_del(VAO **del)
{
    if (*del == NULL)
    {
        return;
    }

    glDeleteVertexArrays(1, &((*del)->ID));
    free(*del);
    *del = NULL;
}

