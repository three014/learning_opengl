#include "shaders.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SHADER_SIZE 256 * 10
#define BUFFER_SIZE 256
#define NULL_CHAR '\0'

typedef struct __VERTEX_OR_FRAGMENT_SHADER
{
    GLuint ID;
    GLenum type;
    char *src_code;
} small_shader;

typedef struct __VERTEX_FRAGMENT_SHADER_PROGRAM
{
    GLuint ID;
} Shader;


char *sh_parse_file(const char *src);
int sh_shader_compile(small_shader **shader);
int sh_prog_compile(Shader **program, small_shader **vert, small_shader **frag);
void sh_shader_del(small_shader **del);

char *sh_parse_file_helper(FILE *in)
{
    int err = OK;
    char *shader = NULL;
    if (!in)
    {
        err = error_callback("STDIO::FILE::FOPEN_FAILED", strerror(errno));
    }
    else
    {
        char buffer[BUFFER_SIZE];
        const int shader_size = MAX_SHADER_SIZE;
        shader = malloc(sizeof *shader * shader_size);
        char *jump = shader;
        
        // Read every line of shader file and copy to 
        // shader string.
        // MAKE SURE NO BUFFER OVERFLOW!! 
        while (!feof(in))
        {
            // Check if the shader string has enough space to 
            // hold the next line of the buffer.
            if (shader_size - (jump - shader) <= BUFFER_SIZE)
            {
                err = error_callback("FILE::READ::NOT_ENOUGH_SPACE",
                        "The input string is not large enough to contain "
                        "the shader information"); 
                break;
            }

            // Get one line of the shader file 
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, in);
            if (feof(in))
            {
                break;
            }

            // Copy the buffer to the end of the shader string 
            strncpy(jump, buffer, shader_size - (jump - shader));

            // Move the shader char pointer to 
            // the null terminating character
            jump = memchr(jump, NULL_CHAR, shader_size);
            if (!jump)
            {
                err = error_callback("FILE::READ::OVERWROTE_NULL_CHAR",
                        "The input string got its null-terminating "
                        "character overwritten."); 
                break;
            }
        }
    }

    if (err == BAD)
    {
        free(shader);
        shader = NULL;
    }

    return shader;
}


/// @brief Opens a file and outputs its contents into a null-terminating string.
/// @param src The full or relative pathname for the file to be read.
/// @return A string containing the file contents, or NULL if something went wrong.
char *sh_parse_file(const char *src)
{
    FILE *in = fopen(src, "r");
    char *shader = sh_parse_file_helper(in);
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
    glShaderSource((*shader)->ID, 1, (const char *const *) &((*shader)->src_code), NULL);

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
        return error_callback("SHADER::VERTEX::COMPILATION_FAILED", info_log);
    }

    return OK;
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
        return error_callback("SHADER::PROGRAM::LINKING_FAILED", info_log);
    }

    return OK;
}

/// @brief Creates and generates a shader program from the specified vertex and fragment files.
/// @param vertex_file The relative or absolute path to vertex file.
/// @param fragment_file The relative or absolute path to fragment file.
/// @param out The outgoing shader program.
/// @return Either 1 for "OK" or 0 for "BAD"
int sh_prog_init(const char *vertex_file, const char *fragment_file, Shader **out)
{
    int err = OK;

    char *vertex_code = NULL; 
    char *fragment_code = NULL;
    Shader *new_program = NULL;
    small_shader *vert = NULL;
    small_shader *frag = NULL;

    vertex_code = sh_parse_file(vertex_file);
    if (!vertex_code)
    {
        err = BAD;
    }

    if (err == OK)
    {
        fragment_code = sh_parse_file(fragment_file);
        if (!fragment_code)
        {
            err = BAD;
        }
    }

    if (err == OK)
    {
        new_program = malloc(sizeof *new_program);
        if (!new_program)
        {
            err = error_callback("SHADER::PROGRAM::ALLOCATION_FAILED", strerror(errno));
        }
    }

    if (err == OK)
    {
        vert = malloc(sizeof *vert);
        if (!vert)
        {
            err = error_callback("SHADER::VERTEX::ALLOCATION_FAILED", strerror(errno));
        }
        else 
        {
            vert->src_code = vertex_code;
            vert->type = GL_VERTEX_SHADER;
            vertex_code = NULL; // Shouldn't be a mem leak, since we're freeing the string in next function
            err = sh_shader_compile(&vert);
        }
    }

    if (err == OK)
    {
        frag = malloc(sizeof *frag);
        if (!frag)
        {
            err = error_callback("SHADER::FRAGMENT::ALLOCATION_FAILED", strerror(errno));
        }
        else 
        {
            frag->src_code = fragment_code;
            frag->type = GL_FRAGMENT_SHADER;
            fragment_code = NULL; // Shouldn't be a mem leak either for the same reason as before
            err = sh_shader_compile(&frag);
        }
    }
    
    if (err = OK)
    {
        err = sh_prog_compile(&new_program, &vert, &frag);
    }

    if (err == BAD)
    {
        if (vertex_code != NULL)
        {
            free(vertex_code);
            vertex_code = NULL;
        }
        if (fragment_code != NULL)
        {
            free(fragment_code);
            fragment_code = NULL;
        }
        if (vert != NULL)
        {
            sh_shader_del(&vert);
        }
        if (frag != NULL)
        {
            sh_shader_del(&frag);
        }
        if (new_program != NULL)
        {
            sh_prog_del(&new_program);
        }
    }

    sh_shader_del(&vert);
    sh_shader_del(&frag);
    *out = new_program;

    return err;
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

void sh_activate(Shader *shader_program)
{
    glUseProgram(shader_program->ID);
}

GLint sh_get_uniloc(Shader *shader_program, const char *var_name)
{
    if (shader_program == NULL)
    {
        return 0;
    }
    return glGetUniformLocation(shader_program->ID, var_name);
}

