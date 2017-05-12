//GLEW must be defined before GLFW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//OTHERS
#include "Shader.h"

// Prototype
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

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

	// Build and Compile our shader program
	Shader ourShader("shaders/default.vert", "shaders/default.frag");

	// CREATE TRIANGLE AND RENDER
	// Create a triangle with normalized (-1 to 1) vertex values
	GLfloat vertices[] = {
		// positions		//colors
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	// Bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	// Bottom left
		0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// Top
	};

	// create vertex buffer objects
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// generate vertex array object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	// set VBO as the vertex buffer meaning everytime we call
	// GL_ARRAY_BUFFER, we will be writing/reading VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copies user defined data stored in vertices into the color buffer
	// GL_STATIC DRAW means we prolly won't change the data mmuch
	// There's also GL_DYNAMIC_DRAW and GL_STREAM_DRAW
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set vertice attributes
	// Position attribute
	// First vertex position / size of vertex / type of vertex components / if you want to normalize / stride=space b/w vertices/ offset of first component of attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // enables the vertex position attribute

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // enables the vertex color attribute

	// unbind the VAO and VBO
	// glBindBuffer(GL_ARRAY_BUFFER, 0); //They don't unbind the VBO in the tutorial for some reason
	glBindVertexArray(0);


	// Loop that will run every frame until something causes termination
	while(!glfwWindowShouldClose(window))
	{
		// Checks if any inputs have been triggered (eg. mouse or keyboard click)
		glfwPollEvents();

		// Rendering Commands go here
		// Clear Colorbuffer  and set background colour to clear to
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Making sure to activate the Shader
		ourShader.Use(); 
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 2nd: starting index, 3rd: num of vertices
		glBindVertexArray(0);

		// Will swap the pointers to the double buffers
		// (where one buffer is displayed and the other one has the next frame being drawn on it)
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW and clear any resources allocated by it
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