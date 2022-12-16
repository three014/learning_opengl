#include "shaders.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parse_file(const char *src)
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

int compile_shader(Shader *program, int type)
{
    if (!(type == VERT || type == FRAG))
    {
        return 0;
    }
    int success;
    char info_log[INFO_LOG_SIZE];

    program->shaders[type].ID = glCreateShader(program->shaders[type].type);
    glShaderSource(program->shaders[type].ID, 
            1, 
            (const char *const *) &(program->shaders[type].src_code), 
            NULL);

    glCompileShader(program->shaders[type].ID);

    free(program->shaders[type].src_code);
    program->shaders[type].src_code = NULL;   
    
    glGetShaderiv(program->shaders[type].ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(program->shaders[type].ID, INFO_LOG_SIZE, NULL, info_log);
        error_callback("SHADER::VERTEX::COMPILATION_FAILED", info_log);
        return 0;
    }

    return 1;
}

int compile_program(Shader **program)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    (*program)->ID = glCreateProgram();
    glAttachShader((*program)->ID, (*program)->shaders[VERT].ID);
    glAttachShader((*program)->ID, (*program)->shaders[FRAG].ID);
    glLinkProgram((*program)->ID);

    glDeleteShader((*program)->shaders[VERT].ID);
    glDeleteShader((*program)->shaders[FRAG].ID);


    glGetProgramiv((*program)->ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog((*program)->ID, 512, NULL, info_log);
        error_callback("SHADER::PROGRAM::LINKING_FAILED", info_log);
        return 0;
    }

    return 1;
}

int create_shader_prog(const char *vertex_file, const char *fragment_file, Shader **shader_out)
{
    char *vertex_code = parse_file(vertex_file);
    if (!vertex_code)
    {
        return 0;
    }
    char *fragment_code = parse_file(fragment_file);
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

    new_program->shaders[VERT].src_code = vertex_code;
    new_program->shaders[VERT].type = GL_VERTEX_SHADER;
    if (!compile_shader(new_program, VERT))
    {
        if (new_program->shaders[VERT].src_code != NULL)
        {
            free(new_program->shaders[VERT].src_code);
            new_program->shaders[VERT].src_code = NULL;
        }
        free(new_program);
        free(fragment_code);
        return 0;
    }

    new_program->shaders[FRAG].src_code = fragment_code;
    new_program->shaders[FRAG].type = GL_FRAGMENT_SHADER;
    if (!compile_shader(new_program, FRAG))
    {
        if (new_program->shaders[VERT].src_code != NULL)
        {
            free(new_program->shaders[VERT].src_code);
            new_program->shaders[VERT].src_code = NULL;
        }
        if (new_program->shaders[FRAG].src_code != NULL)
        {
            free(new_program->shaders[FRAG].src_code);
            new_program->shaders[FRAG].src_code = NULL;
        }
        free(new_program);
    }

    if (!compile_program(&new_program))
    {
        if (new_program->shaders[VERT].src_code != NULL)
        {
            free(new_program->shaders[VERT].src_code);
            new_program->shaders[VERT].src_code = NULL;
        }
        if (new_program->shaders[FRAG].src_code != NULL)
        {
            free(new_program->shaders[FRAG].src_code);
            new_program->shaders[FRAG].src_code = NULL;
        }
        free(new_program);
    }

    *shader_out = new_program;
    return 1;
}

void delete_shader_prog(Shader *del)
{
    if (del == NULL)
    {
        return;
    }

    if (del->shaders[VERT].src_code != NULL)
    {
        free(del->shaders[VERT].src_code);
        del->shaders[VERT].src_code = NULL;
    }

    if (del->shaders[FRAG].src_code != NULL)
    {
        free(del->shaders[FRAG].src_code);
        del->shaders[FRAG].src_code = NULL;
    }
    free(del);
}
