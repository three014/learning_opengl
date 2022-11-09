#include "shaders.h"
#include "lib.h"

#include <stdlib.h>
#include <string.h>

int parse_shader(FILE *in, char **shader)
{
    char buffer[BUFFER_SIZE];
    int shader_size = MAX_SHADER_SIZE;

    /* Check if the input shader string is empty.
     * If not, then exit function with error */
    if (*shader)
    {
        error_callback("SHADER::VERTEX::UNEMPTY_STRING",
                "The input string was not empty -- can't overwrite "
                "existing data.");
        return 0;
    }
    *shader = (char *) malloc(shader_size * sizeof(char));
    char *jump = *shader;

    /* Read every line of shader file and copy to 
     * shader string.
     * MAKE SURE NO BUFFER OVERFLOW!! */
    while (!feof(in))
    {
        /* Check if the shader string has enough space to 
         * hold the next line of the buffer. */
        if (shader_size - (jump - *shader) <= BUFFER_SIZE)
        {
            error_callback("SHADER::VERTEX::NOT_ENOUGH_SPACE",
                    "The input string is not large enough to contain "
                    "the shader information");
            free(shader);
            return 0;
        }

        /* Get one line of the shader file */
        fgets(buffer, BUFFER_SIZE, in);
        if (feof(in))
        {
            break;
        }

        /* Copy the buffer to the end of the shader string */
        strncpy(jump, buffer, shader_size - (jump - *shader));

        /* Move the shader char pointer to 
         * the null terminating character */
        jump = memchr(jump, NULL_CHAR, shader_size);
        if (!jump)
        {
            error_callback("SHADER::VERTEX::OVERWROTE_NULL_CHAR",
                    "The input string got its null-terminating "
                    "character overwritten.");
            free(shader);
            return 0;
        }
    }
    //printf("%s\n", shader);

    return 1;
}

void build_vertex_shader(vertex_shader *vert)
{
    int success;
    char info_log[INFO_LOG_SIZE];
    int parsed_correctly;
    FILE *file_in;

    file_in = fopen(vert->file_loc, "r");
    if (!file_in)
    {
        error_callback("STDIO::FILE::FOPEN_FAILED", strerror(errno));
        return;
    }

    parsed_correctly = parse_shader(file_in, &vert->src_code);
    if (parsed_correctly)
    {
        vert->id = glad_glCreateShader(GL_VERTEX_SHADER);
        glad_glShaderSource(vert->id, 1,
                (const char *const *) &vert->src_code, NULL);

        glad_glCompileShader(vert->id);
        
        glad_glGetShaderiv(vert->id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glad_glGetShaderInfoLog(vert->id, 
                    INFO_LOG_SIZE, NULL, info_log);
            error_callback("SHADER::VERTEX::COMPILATION_FAILED", info_log);
        }
    }
    fclose(file_in);
    free(vert->src_code);
}

void build_fragment_shader(fragment_shader *frag)
{
    int success;
    int parsed_correctly;
    char info_log[INFO_LOG_SIZE];
    FILE *file_in;
    
    file_in = fopen(frag->file_loc, "r");
    if (!file_in)
    {
        error_callback("STDIO::FILE::FOPEN_FAILED", strerror(errno));
        return;
    }

    parsed_correctly = parse_shader(file_in, &frag->src_code);    
    if (parsed_correctly)
    {
        frag->id = glad_glCreateShader(GL_FRAGMENT_SHADER);
        glad_glShaderSource(frag->id, 1, 
                (const char *const *) &frag->src_code, NULL);
        glad_glCompileShader(frag->id);

        glad_glGetShaderiv(frag->id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glad_glGetShaderInfoLog(frag->id, 
                    INFO_LOG_SIZE, NULL, info_log);
            error_callback(
                    "SHADER::FRAGMENT::COMPILATION_FAILED", 
                    info_log);
        }
    }
    fclose(file_in);
    free(frag->src_code);
}

int compile_shaders(unsigned int *vertex_shader,
                    unsigned int *fragment_shader,
                    unsigned int *shader_program)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    *shader_program = glad_glCreateProgram();
    glad_glAttachShader(*shader_program, *vertex_shader);
    glad_glAttachShader(*shader_program, *fragment_shader);
    glad_glLinkProgram(*shader_program);

    glad_glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glad_glGetProgramInfoLog(*shader_program, 512, NULL, info_log);
        error_callback("SHADER::PROGRAM::LINKING_FAILED", info_log);
        return 0;
    }

    return 1;
}
