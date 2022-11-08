#include "shaders.h"
#include "lib.h"

#include <stdlib.h>
#include <string.h>

int parse_shader(FILE *in, char **shader, int shader_size)
{
    char buffer[BUFFER_SIZE];

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

void build_vertex_shader(FILE *file_in,
                         char **vs_str,
                         unsigned int max_strlen,
                         unsigned int* vertex_shader)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    int parse_correctly = parse_shader(file_in, vs_str, max_strlen);
    if (parse_correctly)
    {
        *vertex_shader = glad_glCreateShader(GL_VERTEX_SHADER);
        glad_glShaderSource(*vertex_shader, 1,
                (const char *const *) vs_str, NULL);

        glad_glCompileShader(*vertex_shader);
        
        glad_glGetShaderiv(*vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glad_glGetShaderInfoLog(*vertex_shader, 
                    INFO_LOG_SIZE, NULL, info_log);
            error_callback("SHADER::VERTEX::COMPILATION_FAILED", info_log);
        }
    }
}

void build_fragment_shader(FILE *file_in, 
                           char **fs_str, 
                           unsigned int max_strlen, 
                           unsigned int* fragment_shader)
{
    int success;
    char info_log[INFO_LOG_SIZE];

    int parse_correctly = parse_shader(file_in, fs_str, max_strlen);    
    if (parse_correctly)
    {
        *fragment_shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
        glad_glShaderSource(*fragment_shader, 1, 
                (const char *const *) fs_str, NULL);
        glad_glCompileShader(*fragment_shader);

        glad_glGetShaderiv(*fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glad_glGetShaderInfoLog(*fragment_shader, 
                    INFO_LOG_SIZE, NULL, info_log);
            error_callback(
                    "SHADER::FRAGMENT::COMPILATION_FAILED", 
                    info_log);
        }
    }
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
