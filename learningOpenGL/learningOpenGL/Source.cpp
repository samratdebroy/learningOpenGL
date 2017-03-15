//GLEW must be defined before GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

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

	//////// VERTEX SHADER ////////

	// Get shader source code from file and store shader in C-string
	std::ifstream vertexShaderFile ("triangleVertexShader.txt",std::ifstream::in | std::ifstream::binary);

	GLchar* vertexShaderSource;
	if(vertexShaderFile)
	{
		// Get length of file:
		vertexShaderFile.seekg(0, vertexShaderFile.end); // set position to last char
		int shaderFileLength = vertexShaderFile.tellg();	// get character position as length
		vertexShaderFile.seekg(0, vertexShaderFile.beg); //set position back to beginning
		
		vertexShaderSource = new GLchar[shaderFileLength + 1];
		vertexShaderSource[shaderFileLength] = '\0';

		// Get all characters from the shader file and put it in the C-string
		vertexShaderFile.read(vertexShaderSource, shaderFileLength);

		vertexShaderFile.close(); // close the file
	}else
	{
		std::cout << "File could not be found" << std::endl;
	}


	// store the handle to the vertex shader in a unsined int
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Set shader code of Shader object according to new source
	// the second param represents the number of elements in the string array
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//delete vertexShaderSource; //clear memory

	//ERROR CHECK
	// Check if the compilation was successful
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// If it wasn't succesful then display error message
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//////////// FRAGMENT SHADER ///////

	// Get shader source code from file and store shader in C-string
	std::ifstream fragmentShaderFile("triangleFragmentShader.txt", std::ifstream::in | std::ifstream::binary);

	

	GLchar* fragmentShaderSource;
	if (fragmentShaderFile)
	{
		// Get length of file:
		fragmentShaderFile.seekg(0, fragmentShaderFile.end); // set position to last char
		int shaderFileLength = fragmentShaderFile.tellg();	// get character position as length
		fragmentShaderFile.seekg(0, fragmentShaderFile.beg); //set position back to beginning

		fragmentShaderSource = new GLchar[shaderFileLength + 1];
		fragmentShaderSource[shaderFileLength] = '\0'; // null terminate

		// Get all characters from the shader file and put it in the C-string
		fragmentShaderFile.read(fragmentShaderSource, shaderFileLength);

		fragmentShaderFile.close();
	}
	else
	{
		std::cout << "File could not be found" << std::endl;
	}


	// store the handle to the fragment shader in a unsined int
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Set shader code of Shader object according to new source
	// the second param represents the number of elements in the string array
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//delete fragmentShaderSource; //clear memory

	//ERROR CHECK
	// Check if the compilation was successful
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	// If it wasn't succesful then display error message
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/////// CREATE SHADER PROGRAM AND BIND SHADERS /////
	GLuint shaderProgram;
	shaderProgram = glCreateProgram(); // get ID of program

	// Link the previously compiled shaders into the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // link all attached shaders to a final shader

	// Check if linking worked
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX&FRAGMENT::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete unused shaders since they are inside  the program now
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// CREATE TRIANGLE AND RENDER
	// Create a triangle with normalized (-1 to 1) vertex values
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
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
	// first vertex position / size of vertex / type of vertex components / if you want to normalize / stride=space b/w vertices/ ??
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // enables the vertex attributes


	// unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

		glUseProgram(shaderProgram); // first triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); // 2nd: starting index, 3rd: num of vertices
		glBindVertexArray(0);



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