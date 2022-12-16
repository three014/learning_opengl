#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib.h"
#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float triangle_one[] =
{
    // positions            //colors
    -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 
};

float triangle_two[] = 
{
     0.5f,  0.5f, 0.0f,
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


int main()
{
    int success;
    char infoLog[512];
    unsigned int VBO[2], VAO[2];
    FILE *file_in;

    Shader *hello = NULL;
    Shader *hi = NULL;

    
    /* Initialize the library */
    if (!glfwInit())
    {
        fprintf(stderr, "Init failed\n");
        return -1;
    }
    info_callback("GLFW::INIT::SUCCESS");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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


    /* Build and compile our shader program */
    success = create_shader_prog("../resources/shaders/customshader.vert", 
                                 "../resources/shaders/vertexColor.frag", 
                                 &hello);
    if (!success)
    {
        delete_shader_prog(hello);
        delete_shader_prog(hi);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    success = create_shader_prog("../resources/shaders/shader.vert", 
                                 "../resources/shaders/uniform.frag", 
                                 &hi);
    if (!hi)
    {
        delete_shader_prog(hello);
        delete_shader_prog(hi);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    info_callback("SHADERS::COMPILE::SUCCESS");
    /* Free memory */

    /* Set up vertex data (and buffer(s)) and configure vertex attributes */
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    /* 1st Triangle */
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_one), triangle_one, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    /* 2nd Triangle */
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_two),
            triangle_two, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, 
            GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


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
        glUseProgram(hello->ID);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        /* Draw 2nd Triangle */
        /* Activate 2nd shader */
        glUseProgram(hi->ID);
        
        /* Update the uniform color */
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(hi->ID, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        /* Render the triangle */
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(hello->ID);
    glDeleteProgram(hi->ID);
    delete_shader_prog(hello);
    delete_shader_prog(hi);

    glfwDestroyWindow(window);
    glfwTerminate(); // Currently gives a SegFault on Linux,
                     // but not on Windows?
    return 0;
}

