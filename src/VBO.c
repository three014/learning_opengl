#include "VBO.h"
#include "lib.h"

#include <stdlib.h>
#include <string.h>

typedef struct __VBO_STRUCT {
    GLuint ID;
} VBO;


VBO *vbo_init(GLfloat *vertices, GLsizeiptr size) {
    VBO *vbo = malloc(sizeof *vbo);
    if (vbo == NULL) {
        error_callback("VBO::INIT::ALLOCATION_FAILED", strerror(errno));
        return NULL;
    }

    glGenBuffers(1, &(vbo->ID));
    glBindBuffer(GL_ARRAY_BUFFER, vbo->ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    return vbo;
}

void vbo_bind(VBO *self) {
    if (self == NULL) {
        return NULL;
    }

    glBindBuffer(GL_ARRAY_BUFFER, self->ID);
}

void vbo_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_del(VBO **del) {
    if (*del == NULL) {
        return;
    }

    glDeleteBuffers(1, &((*del)->ID));

    free(*del);
    *del = NULL;
}
