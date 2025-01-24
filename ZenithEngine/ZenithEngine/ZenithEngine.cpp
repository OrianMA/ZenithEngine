#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1, 1, 1, 1.0f);\n"
"}\n\0";

int main() {
    // Initialisation de GLFW
    if (!glfwInit()) {
        std::cerr << "Échec de l'initialisation de GLFW!" << std::endl;
        return -1;
    }

    // the point (vertices) of the triangle
    // Vertices coordinates
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
        0.5f, 0.5f * float(sqrt(3)) / 3, 0.0f, // Upper corner
        -0.5f, 0.5f * float(sqrt(3)) / 3, 0.0f, // Inner left
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2, // Lower left triangle
        0, 3, 2, // Upper triangle
    };

    //GLfloat vertices[] = {
    //    -.5f, -.5f * float(sqrt(3)) / 3,0,
    //    .5f, -.5f * float(sqrt(3)) / 3,0,
    //    .5f,.5f * float(sqrt(3)) * 2 / 3, 0,
    //};



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
    
    //Specify the viewport of Opengl in the window
    //In this case the viewport foes from x = 0, y = 0


    
       // Create reference containers for the Vertex Array Objet and the Vertex Buffer Object
    GLuint VAO[1], EBO;

    // generate the Vertex Array Objects and the Vertex Buffer Object with only 1 object each
    glGenVertexArrays(1, &VAO[0]),
    


    // Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAO[0]);





    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the vertex Attribute so OpenGl knos to use it
    glEnableVertexAttribArray(0);

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
        // Code de rendu OpenGL ici (par exemple, remplir la fenêtre de noir)

        // Afficher la fenêtre
        //std::cerr << colorResult << std::endl;
        glViewport(0, 0, 800, 800);
        
  /*      if (time - prev_time >= 0.1f) {

        }*/
        
        colorResult += deltaTime * angle;
        //Clean the back buffer and assign the new color to it
        glClearColor(float(sin(colorResult)), float(cos(colorResult)), float(tan(colorResult)), 1);
        //Swap the back buffer with the front buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Tell OpenGL which Shader Program we want tu use
        
        // Bind the VAO so OpenGL know to use it
        glBindVertexArray(VAO[0]);
        // Draw the triangle using the GL_TRIANGLE primitive
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);

        deltaTime -= prev_time;
        //glfwSetTime(0);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    glDeleteVertexArrays(1, &VAO[0]);
    

    // Nettoyer et fermer
    glfwDestroyWindow(window);
    //Terminate GLFW before ending the program
    glfwTerminate();

    return 0;
}
