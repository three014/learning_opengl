#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib.h"
#include "shaders.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_SHADER_SIZE 256 * 10

float triangle_one[] =
{
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

float triangle_two[] = 
{
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

float rectangle_thing[] =
{
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, // top left
};
unsigned int rectangle_indices[] =
{
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};


char *vertexShaderSource;
char *fragmentShaderSource1;
char *fragmentShaderSource2;

int main()
{
    int success;
    char infoLog[512];
    unsigned int VBO[2], VAO[2];
    unsigned int vertexShader;
    unsigned int fragmentShader[2];
    unsigned int shaderProgram[2];
    FILE *file_in;


    //glfwSetErrorCallback(error_callback);
    //if (glfwPlatformSupported(GLFW_PLATFORM_WIN32))
    //    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WIN32);

    /* Initialize the library */
    if (!glfwInit())
    {
        fprintf(stderr, "Init failed\n");
        return -1;
    }
    info_callback("GLFW::INIT::SUCCESS");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
            "Guwah", NULL, NULL);
    if (!window)
    {
        error_callback("WINDOW::CREATE::FAILURE", 
                "Failed to create window"); 
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    info_callback("WINDOW::CREATE::SUCCESS");


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        error_callback("OPENGL::INIT::FAILURE", 
                "Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    info_callback("OPENGL::INIT::SUCCESS");


    /* --------------------------------------------------------------------- */


    /* Build and Compile our shader program */
    
    // Vertex Shader
    file_in = fopen("../src/shaders/shader.vert", "r");
    if (file_in)
    {
        build_vertex_shader(file_in, &vertexShaderSource, 
                MAX_SHADER_SIZE, &vertexShader);
        fclose(file_in);
        file_in = NULL;
    }
    else 
    {
        error_callback("STDIO::FILE::FOPEN_FAILED", strerror(errno));
    }

    // 1st Fragment Shader
    file_in = fopen("../src/shaders/orange.frag", "r");
    if (file_in)
    {
        build_fragment_shader(file_in, &fragmentShaderSource1, 
                MAX_SHADER_SIZE, (fragmentShader + 0));
        fclose(file_in);
        file_in = NULL;
    }
    else 
    {
        error_callback("STDIO::FILE::FOPEN_FAILED", strerror(errno));
    }

    file_in = fopen("../src/shaders/uniform.frag", "r");
    if (file_in)
    {
        build_fragment_shader(file_in, &fragmentShaderSource2, 
                MAX_SHADER_SIZE, (fragmentShader + 1));
        fclose(file_in);
        file_in = NULL;
    }
    else
    {
        error_callback("STDIO::FILE::FOPEN_FAILED", strerror(errno));
    }

    /* Link shaders */
    compile_shaders(&vertexShader, fragmentShader + 0, 
            shaderProgram + 0);
    compile_shaders(&vertexShader, fragmentShader + 1, 
            shaderProgram + 1);
    free(vertexShaderSource);
    free(fragmentShaderSource1);
    free(fragmentShaderSource2);
    glad_glDeleteShader(vertexShader);
    glad_glDeleteShader(fragmentShader[0]);
    glad_glDeleteShader(fragmentShader[1]);

    /* Set up vertex data (and buffer(s)) and configure vertex attributes */
    glad_glGenVertexArrays(2, VAO);
    glad_glGenBuffers(2, VBO);

    /* 1st Triangle */
    glad_glBindVertexArray(VAO[0]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_one), 
            triangle_one, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
            3 * sizeof(float), (void *) 0);
    glad_glEnableVertexAttribArray(0);
    glad_glBindVertexArray(0);

    /* 2nd Triangle */
    glad_glBindVertexArray(VAO[1]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_two),
            triangle_two, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, 
            GL_FALSE, 0, (void *) 0);
    glad_glEnableVertexAttribArray(0);
    glad_glBindVertexArray(0);


    /* --------------------------------------------------------------------- */


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Input */       
        processInput(window);

        /* Rendering commands */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* RECTANGLE */
        /* Draw 1st Triangle */
        glad_glUseProgram(shaderProgram[0]);
        glad_glBindVertexArray(VAO[0]);
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);
        /* Draw 2nd Triangle */
        /* Activate 2nd shader */
        glad_glUseProgram(shaderProgram[1]);
        /* Update the uniform color */
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = 
                glad_glGetUniformLocation(shaderProgram[1], 
                        "ourColor");
        glad_glUniform4f(vertexColorLocation, 0.0f, 
                greenValue, 0.0f, 1.0f);
        /* Render the triangle */
        glad_glBindVertexArray(VAO[1]);
        glad_glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glad_glDeleteVertexArrays(2, VAO);
    glad_glDeleteBuffers(2, VBO);
    glad_glDeleteProgram(shaderProgram[0]);
    glad_glDeleteProgram(shaderProgram[1]);

    glfwDestroyWindow(window);
    glfwTerminate(); // Currently gives a SegFault
    return 0;
}

