//GLEW must be defined before GLFW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//OTHERS
#include "stb_image.h"
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

	// CREATE RECTANGLE AND RENDER
	// Create a rectangle with normalized (-1 to 1) vertex values
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// create vertex buffer objects,vertex array objects and element buffer object
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	/// Bind VBO (copy vertices array into VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copies user defined data stored in vertices into the color buffer
	// GL_STATIC DRAW means we prolly won't change the data much
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/// Bind the Element Buffer Object (copy index array into EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//////// Set vertice attributes ///////////

	//Position attribute
	// First vertex position / size of vertex / type of vertex components / if you want to normalize / stride=space b/w vertices/ offset of first component of attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // enables the vertex position attribute
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // enables the vertex color attribute
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); // enables the vertex texture attribute

	//////////////////// TEXTURES //////////////////

	// Create Texture ID
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture repeated on s & t axes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering when minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // or when magnifying
	// Import the texture from a file
	int tex_width, tex_height, nrChannels;
	unsigned char *data = stbi_load("textures/container.jpg", &tex_width, &tex_height, &nrChannels, 0); // load from file
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Gen the texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // Free image memory

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

		// Bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// Making sure to activate the Shader
		ourShader.Use(); 
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Will swap the pointers to the double buffers
		// (where one buffer is displayed and the other one has the next frame being drawn on it)
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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