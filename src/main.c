#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib.h"
#include "shaders.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "texture.h"
#include "stb/stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define SQRT3 1.7320508075688772f

/* Anime Watch Order
   1. Yuri on Ice
   2. The Quintessential Quintuplets
   3. Skate the Infinity
   4. Paradise Kiss
*/

GLfloat triangle_one[] = {
    // positions            //colors
    -0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
};

GLfloat triangle_two[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

GLfloat rectangle_thing[] = {
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, // top left
};

GLuint rectangle_indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

GLfloat three_triangles_color[] = { 
    //            COORDINATES               /         COLORS        //
    -0.5f,  -0.5f * (SQRT3) / 3,     0.0f,      0.8f, 0.3f,  0.02f, // Lower left corner 
     0.5f,  -0.5f * (SQRT3) / 3,     0.0f,      0.8f, 0.3f,  0.02f, // Lower right corner
     0.0f,   0.5f * (SQRT3) * 2 / 3, 0.0f,      1.0f, 0.6f,  0.32f, // Upper corner
    -0.25f,  0.5f * (SQRT3) / 6,     0.0f,      0.9f, 0.45f, 0.17f, // Inner left
     0.25f,  0.5f * (SQRT3) / 6,     0.0f,      0.9f, 0.45f, 0.17f, // Inner right
     0.0f,  -0.5f * (SQRT3) / 3,     0.0f,      0.8f, 0.3f,  0.02f, // Inner down
};

GLuint three_triangles_indices[] = {
    0, 3, 5, // Lower left triangle 
    3, 2, 4, // Lower right triangle
    5, 4, 1, // Upper triangle      
};

GLfloat square[] = {
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
};

GLuint square_indices[] = {
    0, 2, 1, 
    0, 3, 2, 
};


int main() {
    unsigned int err = OK;
    GLFWwindow *window = NULL;
    Shader *hello = NULL;
    VAO *vao1 = NULL;
    VBO *vbo1 = NULL;
    EBO *ebo1 = NULL;
    Texture *pop_cat = NULL;

    GLuint scale = 0;
    unsigned int set_scale = FALSE;

    unsigned int show_tex = FALSE;

    // Initialize the library
    if (!glfwInit()) {
        fprintf(stderr, "Init failed\n");
        err = BAD;
    }
    else {
        err = info_callback("GLFW::INIT::SUCCESS");
    }

    if (err == OK) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a windowed mode window and its OpenGL context 
        window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Guwah", NULL, NULL);
        if (!window) {
            err = error_callback("WINDOW::CREATE::FAILURE", "Failed to create window"); 
        }
        else {
            glfwMakeContextCurrent(window); 
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            err = info_callback("WINDOW::CREATE::SUCCESS");
        }
    }
    
    if (err == OK) {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            err = error_callback("OPENGL::INIT::FAILURE", "Failed to initialize GLAD");
        }
        else {
            err = info_callback("OPENGL::INIT::SUCCESS");
        }
    }


    /* --------------------------------------------------------------------- */


    if (err == OK) {
        // Creates Shader object using shaders shader.vert and shader.frag
        if (!sh_prog_init("../resources/shaders/shader.vert",
                          "../resources/shaders/shader.frag",
                          &hello)) {
            err = BAD;
        }
        else {
            err = info_callback("SHADERS::COMPILE::SUCCESS");
        }
    }

    if (err == OK) {
        // Generates Vertex Array Object and binds it
        vao1 = vao_init();
        if (!vao1)
        {
            err = BAD;
        }
        else {
            vao_bind(vao1);
            err = OK;
        }
    }

    if (err == OK) {
        // Generates Vertex Buffer Object and links it to vertices
        vbo1 = vbo_init(square, sizeof square);
        if (!vbo1)
        {
            err = BAD;
        }
    }

    if (err == OK) {
        // Generates Element Buffer Object and links it to indices
        ebo1 = ebo_init(square_indices, sizeof square_indices);
        if (!ebo1)
        {
            err = BAD;
        }
    }

    if (err == OK) {
        // Links VBO to VAO
        vao_linkattrib(vao1, vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *) 0);
        vao_linkattrib(vao1, vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        vao_linkattrib(vao1, vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *) (6 * sizeof(float)));

        scale = 0;
        set_scale = sh_get_uniloc(hello, "scale", &scale);

        // Textures
        pop_cat = tex_init("../resources/textures/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        if (!pop_cat) {
            err = BAD;
        }
        else {
            show_tex = tex_unit(hello, "tex0", 0);
        }

    }

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* --------------------------------------------------------------------- */

    
    if (err == OK) {
        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Input
            processInput(window);


            // Rendering commands 
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            // DRAW
            sh_activate(hello);

            if (set_scale) {
                glUniform1f(scale, 0.5f);
            }

            if (show_tex) {
                tex_bind(pop_cat);
            }

            vao_bind(vao1);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            tex_unbind(pop_cat);
            

            // Swap front and back buffers
            glfwSwapBuffers(window);


            // Poll for and process events
            glfwPollEvents();
        }
    }

    vao_del(&vao1);
    vbo_del(&vbo1);
    ebo_del(&ebo1);
    sh_prog_del(&hello);
    tex_del(&pop_cat);
    if (window != NULL) {
        glfwDestroyWindow(window);
    }
    glfwTerminate(); // Currently gives a SegFault on Linux,
                     // but not on Windows?
    return err;
}

