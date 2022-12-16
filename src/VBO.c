#include "VBO.h"
#include "lib.h"

#include <stdlib.h>

int vbo_init(GLfloat *vertices, GLsizeiptr size, VBO **out)
{
    if (*out != NULL)
    {
        error_callback("VBO::INIT::FAILURE", 
                "VBO Pointer was not NULL, might have data attached to it.");
        return 0;
    }

    *out = malloc(sizeof **out);

    glGenBuffers(1, &((*out)->ID));
    glBindBuffer(GL_ARRAY_BUFFER, (*out)->ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    
    return 1;
}

void vbo_bind(VBO *self)
{
    glBindBuffer(GL_ARRAY_BUFFER, self->ID);
}

void vbo_unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_del(VBO **del)
{
    if (*del == NULL)
    {
        return;
    }

    free(*del);
    *del = NULL;
}
