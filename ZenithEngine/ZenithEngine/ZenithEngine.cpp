#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Initialisation de GLFW
    if (!glfwInit()) {
        std::cerr << "Échec de l'initialisation de GLFW!" << std::endl;
        return -1;
    }

    // Créer une fenêtre GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Mon projet OpenGL", NULL, NULL);
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

    // Boucle de rendu
    while (!glfwWindowShouldClose(window)) {
        // Code de rendu OpenGL ici (par exemple, remplir la fenêtre de noir)
        glClear(GL_COLOR_BUFFER_BIT);

        // Afficher la fenêtre
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Nettoyer et fermer
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
