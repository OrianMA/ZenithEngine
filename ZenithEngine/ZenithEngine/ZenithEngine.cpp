#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

double deltaTime() {
    double prev_time = glfwGetTime();
    glFinish();
    double deltaTime = glfwGetTime() - prev_time;
    return deltaTime;
}

double fixedDeltaTime() {

}

int main() {
    // Initialisation de GLFW
    if (!glfwInit()) {
        std::cerr << "Échec de l'initialisation de GLFW!" << std::endl;
        return -1;
    }

    // Créer une fenêtre GLFW
    GLFWwindow* window = glfwCreateWindow(width, height, "Zenith window", NULL, NULL);
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

    glViewport(0, 0, width, height);

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*) (3* sizeof(float)));
    VAO1.LinkAttribute(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*) (6* sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    GLuint uniId = glGetUniformLocation(shaderProgram.ID, "scale");

    // Bind both the VAO and VBO to 0 so that we don't accidentally modify the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    float angle = .1f;
    float colorResult = 0;

    float textureChangeDelay = 1.0f;
    float tempDelay = 0;

    bool catSwap = false;

    //Texture
    Texture cat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    cat.texUnit(shaderProgram, "tex0", 0);

    Texture newCat("pop_cat_close.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    newCat.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);    

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // Boucle de rendu
    while (!glfwWindowShouldClose(window)) {
        double prev_time = glfwGetTime();
        //double deltaTime = glfwGetTime() - prev_time;




        colorResult += deltaTime() * angle;
        //Clean the back buffer and assign the new color to it
        glClearColor(float(sin(colorResult)), float(cos(colorResult)), float(tan(colorResult)), 1);
        //Swap the back buffer with the front buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // Tell OpenGL which Shader Program we want tu use
        shaderProgram.Activate();

        camera.Inputs(window);  
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        if (tempDelay > textureChangeDelay) {
            catSwap = !catSwap;
            tempDelay = 0;
        }
        else {
            std::cout << tempDelay << std::endl;
            if (catSwap) {
                newCat.Bind();
            }
            else {
                cat.Bind();
            }
        }

        tempDelay += deltaTime() * 300;

        // Bind the VAO so OpenGL know to use it
        VAO1.Bind();
        // Draw the triangle using the GL_TRIANGLE primitive
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);

       
        //glfwSetTime(0);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    cat.Delete();

    // Nettoyer et fermer
    glfwDestroyWindow(window);
    //Terminate GLFW before ending the program
    glfwTerminate();

    return 0;
}



