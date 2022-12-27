#include "EBO.h"
#include "lib.h"

#include <stdlib.h>
#include <string.h>

typedef struct __EBO_STRUCT {
    GLuint ID;
} EBO;


EBO *ebo_init(GLuint *indices, GLsizeiptr size) {
    EBO *ebo = malloc(sizeof *ebo);
    if (ebo == NULL) {
        error_callback("EBO::INIT::ALLOCATION_FAILED", strerror(errno));
        return NULL;
    }

    glGenBuffers(1, &(ebo->ID));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    
    return ebo;
}

void ebo_bind(EBO *self) {
    if (self == NULL) {
        return;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ID);
}

void ebo_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_del(EBO **del) {
    if (*del == NULL) {
        return;
    }

    glDeleteBuffers(1, &((*del)->ID));

    free(*del);
    *del = NULL;
}
