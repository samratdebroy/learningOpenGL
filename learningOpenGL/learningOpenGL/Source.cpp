//GLEW must be defined before GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Prototype
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	glfwInit(); //initialize GLFW
	// Specify you will be using OpenGL 4.4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	// Specify you only want to use core OpenGL and throw error at legacy function use
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// specify user should not be able to resize window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/// CREATE A WINDOW
	// Create window of 800x600 pixels titled LearnOpenGL
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

	// If window wasn't properly created, then terminate
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // create the context

	// Set the callback functions for keyboard presses
	glfwSetKeyCallback(window, key_callback);

	// Setting to true ensures we use more modern techniques for OpenGL funcationality
	glewExperimental = GL_TRUE;
	// Check if you properly initialized GLEW
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Get the width and height of the window defined before
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// Set origin to lower-left corner of window and define size.
	glViewport(0, 0, width, height);

	// Loop that will run every frame until something causes termination
	while(!glfwWindowShouldClose(window))
	{
		// Checks if any inputs have been triggered (eg. mouse or keyboard click)
		glfwPollEvents();

		// Rendering Commands go here
		// ...

		// Clear Colorbuffer  and set background colour to clear to
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Will swap the pointers to the double buffers
		// (where one buffer is displayed and the other one has the next frame being drawn on it)
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;

}

/**
 * Whenever a user pressed a key, GLFW calls this function and fills in the proper arguments for you to process.
 * @param key specifies which key was pressed
 * @param action specifies an action (ex: if press or release of key)
 * @param mode specifies is superkeys like alt, shift or control have been pressed
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}