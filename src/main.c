#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib.h"
#include "shaders.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SQRT3 1.7320508075688772f

GLfloat triangle_one[] =
{
    // positions            //colors
    -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 
};

GLfloat triangle_two[] = 
{
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

GLfloat rectangle_thing[] =
{
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, // top left
};
GLuint rectangle_indices[] =
{
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};


GLfloat three_triangles_color[] =
{ //            COORDINATES                 /         COLORS        //
    -0.5f,  -0.5f * (SQRT3) / 3,     0.0f,      0.8f, 0.3f,  0.02f, // Lower left corner
     0.5f,  -0.5f * (SQRT3) / 3,     0.0f,      0.8f, 0.3f,  0.02f, // Lower right corner
     0.0f,   0.5f * (SQRT3) * 2 / 3, 0.0f,      1.0f, 0.6f,  0.32f, // Upper corner
    -0.25f,  0.5f * (SQRT3) / 6,     0.0f,      0.9f, 0.45f, 0.17f, // Inner left
     0.25f,  0.5f * (SQRT3) / 6,     0.0f,      0.9f, 0.45f, 0.17f, // Inner right
     0.0f,  -0.5f * (SQRT3) / 3,     0.0f,      0.8f, 0.3f,  0.02f, // Inner down
};


int main()
{
    int success;
    char infoLog[512]; 
    FILE *file_in;

    

    
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
        error_callback("WINDOW::CREATE::FAILURE", "Failed to create window"); 
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    info_callback("WINDOW::CREATE::SUCCESS");


    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        error_callback("OPENGL::INIT::FAILURE", "Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    info_callback("OPENGL::INIT::SUCCESS");


    /* --------------------------------------------------------------------- */

    
    // Creates Shader object using shaders shader.vert and uniform.frag
    Shader *hello = NULL;
    if (!sh_prog_init("../resources/shaders/shader.vert",
                      "../resources/shaders/shader.frag",
                      &hello))
    {
        glfwDestroyWindow(window);
        sh_prog_del(&hello);
        glfwTerminate();
        return 1;
    }

    info_callback("SHADERS::COMPILE::SUCCESS");

    // Generates Vertex Array Object and binds it
    VAO *vao1 = vao_init();
    vao_bind(vao1);

    // Generates Vertex Buffer Object and links it to vertices
    VBO *vbo1 = vbo_init(rectangle_thing, sizeof rectangle_thing);

    // Generates Element Buffer Object and links it to indices
    EBO *ebo1 = ebo_init(rectangle_indices, sizeof rectangle_indices);
    
    // Links VBO to VAO
    vao_linkattrib(vao1, vbo1, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        sh_activate(hello);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();
    }

    vao_del(&vao1);
    vbo_del(&vbo1);
    ebo_del(&ebo1);
    sh_prog_del(&hello);

    glfwDestroyWindow(window);
    glfwTerminate(); // Currently gives a SegFault on Linux,
                     // but not on Windows?
    return 0;
}

