#include "shaders.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHADER_SIZE 256 * 10
#define BUFFER_SIZE 256
#define NULL_CHAR '\0'

#define NUM_SMALL_SHADERS 2

typedef struct __VERTEX_OR_FRAGMENT_SHADER
{
    GLuint ID;
    GLenum type;
    char *src_code;
} small_shader;


char *sh_parse_file(const char *src);
int sh_shader_compile(small_shader **shader);
int sh_prog_compile(Shader **program, small_shader **vert, small_shader **frag);
void sh_shader_del(small_shader **del);


char *sh_parse_file(const char *src)
{
    char buffer[BUFFER_SIZE];
    const int shader_size = MAX_SHADER_SIZE;
    FILE *in = fopen(src, "r");
    if (!in)
    {
        error_callback("STDIO::FILE::FOPEN_FAILED", strerror(errno));
        return NULL;
    }

    char *shader = malloc(sizeof *shader * shader_size);
    char *jump = shader;

    /* Read every line of shader file and copy to 
     * shader string.
     * MAKE SURE NO BUFFER OVERFLOW!! */
    while (!feof(in))
    {
        /* Check if the shader string has enough space to 
         * hold the next line of the buffer. */
        if (shader_size - (jump - shader) <= BUFFER_SIZE)
        {
            error_callback("SHADER::VERTEX::NOT_ENOUGH_SPACE",
                    "The input string is not large enough to contain "
                    "the shader information");
            free(shader);
            fclose(in);
            return NULL;
        }

        /* Get one line of the shader file */
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, in);
        if (feof(in))
        {
            break;
        }

        /* Copy the buffer to the end of the shader string */
        strncpy(jump, buffer, shader_size - (jump - shader));

        /* Move the shader char pointer to 
         * the null terminating character */
        jump = memchr(jump, NULL_CHAR, shader_size);
        if (!jump)
        {
            error_callback("SHADER::VERTEX::OVERWROTE_NULL_CHAR",
                    "The input string got its null-terminating "
                    "character overwritten.");
            free(shader);
            fclose(in);
            return NULL;
        }
    }

    fclose(in);
    return shader;
}

int sh_shader_compile(small_shader **shader)
{
    if (*shader == NULL)
    {
        return 0;
    }
    int success;
    char info_log[INFO_LOG_SIZE];

    (*shader)->ID = glCreateShader((*shader)->type);
    glShaderSource((*shader)->ID, 
            1, 
            (const char *const *) &((*shader)->src_code), 
            NULL);

    glCompileShader((*shader)->ID);

    if ((*shader)->src_code != NULL)
    {
        free((*shader)->src_code);
        (*shader)->src_code = NULL;   
    }
    
    glGetShaderiv((*shader)->ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog((*shader)->ID, INFO_LOG_SIZE, NULL, info_log);
        error_callback("SHADER::VERTEX::COMPILATION_FAILED", info_log);
        return 0;
    }

    return 1;
}

int sh_prog_compile(Shader **program, small_shader **vert, small_shader **frag)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    (*program)->ID = glCreateProgram();
    glAttachShader((*program)->ID, (*vert)->ID);
    glAttachShader((*program)->ID, (*frag)->ID);
    glLinkProgram((*program)->ID);

    glDeleteShader((*vert)->ID);
    glDeleteShader((*frag)->ID);


    glGetProgramiv((*program)->ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog((*program)->ID, 512, NULL, info_log);
        error_callback("SHADER::PROGRAM::LINKING_FAILED", info_log);
        return 0;
    }

    return 1;
}

int sh_prog_init(const char *vertex_file, const char *fragment_file, Shader **out)
{
    char *vertex_code = sh_parse_file(vertex_file);
    if (!vertex_code)
    {
        return 0;
    }
    char *fragment_code = sh_parse_file(fragment_file);
    if (!fragment_code)
    {
        free(vertex_code);
        return 0;
    }

    Shader *new_program = malloc(sizeof *new_program);
    if (!new_program)
    {
        error_callback("SHADER::PROGRAM::ALLOCATION_FAILED", strerror(errno));
        free(vertex_code);
        free(fragment_code);
        return 0;
    }

    small_shader *vert = malloc(sizeof *vert);
    if (!vert)
    {
        error_callback("SHADER::VERTEX::ALLOCATION_FAILED", strerror(errno));
        sh_prog_del(&new_program);
        free(vertex_code);
        free(fragment_code);
        return 0;
    }
    vert->src_code = vertex_code;
    vert->type = GL_VERTEX_SHADER;
    if (!sh_shader_compile(&vert))
    {
        sh_prog_del(&new_program);
        sh_shader_del(&vert);
        free(fragment_code);
        return 0;
    }

    small_shader *frag = malloc(sizeof *frag);
    if (!frag)
    {
        error_callback("SHADER::FRAGMENT::ALLOCATION_FAILED", strerror(errno));
        sh_prog_del(&new_program);
        sh_shader_del(&vert);
        free(fragment_code);
        return 0;
    }
    frag->src_code = fragment_code;
    frag->type = GL_FRAGMENT_SHADER;
    if (!sh_shader_compile(&frag))
    {
        sh_prog_del(&new_program);
        sh_shader_del(&vert);
        sh_shader_del(&frag);
        return 0;
    }

    if (!sh_prog_compile(&new_program, &vert, &frag))
    {
        sh_prog_del(&new_program);
        sh_shader_del(&vert);
        sh_shader_del(&frag);
        return 0;
    }

    sh_shader_del(&vert);
    sh_shader_del(&vert);
    *out = new_program;

    return 1;
}

void sh_shader_del(small_shader **del)
{
    if (*del == NULL)
    {
        return;
    }

    glDeleteShader((*del)->ID);
    
    if ((*del)->src_code != NULL)
    {
        free((*del)->src_code);
        (*del)->src_code = NULL;
    }
    free(*del);
    *del = NULL;
}

void sh_prog_del(Shader **del)
{
    if (*del == NULL)
    {
        return;
    }

    glDeleteProgram((*del)->ID);

    free(*del);
    *del = NULL;
}
