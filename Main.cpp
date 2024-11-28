#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main() {
	glfwInit();
	//Tell him what version we use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create and check window (with name "WindowDeLaCasa")
	GLFWwindow* window = glfwCreateWindow(800, 800, "ZenithWindow", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Introduce the window into the current context 
	glfwMakeContextCurrent(window);

	//Load Glad for openGl
	gladLoadGL();


	//Tell him the windows size proprety, with start with the top right corner
	glViewport(0, 0, 800, 800);

	//Background color (blue)
	glClearColor(0.07f, .13f, .17, 1);
	//Clean the back buffer and assign the new color to it 
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer 
	glfwSwapBuffers(window);


	//Main while loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}

	//Terminate the program, which destroy the window
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}