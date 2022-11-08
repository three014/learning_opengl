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
