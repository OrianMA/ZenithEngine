#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// Vertices coordinates
GLfloat vertices[] =
{ //               COORDINATES                  /     COLORS           //
    -0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
     0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
     0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
    -0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
     0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
     0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
};
// Indices for vertices order
GLuint indices[] =
{
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Lower right triangle
    5, 4, 1 // Upper triangle
};


int main() {
    // Initialisation de GLFW
    if (!glfwInit()) {
        std::cerr << "Échec de l'initialisation de GLFW!" << std::endl;
        return -1;
    }

    // Créer une fenêtre GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Zenith window", NULL, NULL);
    if (!window) {
        std::cerr << "Échec de la création de la fenêtre GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialiser le contexte OpenGL avec Glad
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Échec du chargement de GLAD!" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 800);

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*) (3* sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    GLuint uniId = glGetUniformLocation(shaderProgram.ID, "scale");

    // Bind both the VAO and VBO to 0 so that we don't accidentally modify the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    float angle = 10000;
    float colorResult = 0;

    // Boucle de rendu
    while (!glfwWindowShouldClose(window)) {
        float prev_time = float(glfwGetTime());
        float deltaTime = glfwGetTime() - prev_time;

        
        colorResult += deltaTime * angle;
        //Clean the back buffer and assign the new color to it
        glClearColor(float(sin(colorResult)), float(cos(colorResult)), float(tan(colorResult)), 1);
        //Swap the back buffer with the front buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Tell OpenGL which Shader Program we want tu use
        shaderProgram.Activate();
        glUniform1f(uniId, 0.5f);
        // Bind the VAO so OpenGL know to use it
        VAO1.Bind();
        // Draw the triangle using the GL_TRIANGLE primitive
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);

        deltaTime -= prev_time;
        //glfwSetTime(0);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // Nettoyer et fermer
    glfwDestroyWindow(window);
    //Terminate GLFW before ending the program
    glfwTerminate();

    return 0;
}
