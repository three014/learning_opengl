#include "texture.h"
#include "lib.h"
#include "stb/stb_image.h"

#include <stdlib.h>
#include <string.h>

typedef struct __TEXTURE_STRUCT {
    GLuint ID;
    GLenum type;
} Texture;

Texture *tex_init(const char *image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type) {
    Texture *tex = malloc(sizeof *tex);
    if (tex == NULL) {
        error_callback("TEXTURE::INIT::ALLOCATION_FAILED", strerror(errno));
        return NULL;
    }

    tex->type = tex_type;

    int img_width, img_height, num_col_ch;
    stbi_set_flip_vertically_on_load(TRUE);
    unsigned char *bytes = stbi_load(image, &img_width, &img_height, &num_col_ch, 0);
    if (bytes == NULL) {
        error_callback("TEXTURE::IMAGE::LOAD_FAILED", "Unable to load the texture data; File may not exist.");
        free(tex);
        return NULL;
    }

    glGenTextures(1, &tex->ID);
    glActiveTexture(slot);
    glBindTexture(tex_type, tex->ID);

    glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(tex_type, 0, GL_RGBA, img_width, img_height, 0, format, pixel_type, bytes);
    glGenerateMipmap(tex_type);

    stbi_image_free(bytes);
    glBindTexture(tex_type, 0);

    return tex;
}

unsigned int tex_unit(Shader *shader, const char *uniform, GLuint unit) {
    GLuint tex0_uni = 0; 
    unsigned int err = sh_get_uniloc(shader, uniform, &tex0_uni);
    if (err == OK) {
        sh_activate(shader);
        glUniform1f(tex0_uni, unit);
    }

    return err;
}

void tex_bind(Texture *texture) {
    if (texture == NULL) {
        return;
    }

    glBindTexture(texture->type, texture->ID);
}

void tex_unbind(Texture *texture) {
    if (texture == NULL) {
        return;
    }
    
    glBindTexture(texture->type, 0);
}

void tex_del(Texture **texture) {
    if (*texture == NULL) {
        return;
    }

    glDeleteTextures(1, &((*texture)->ID));

    free(*texture);
    *texture = NULL;
}

