//GLEW must be defined before GLFW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//OTHERS
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Prototype
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Window dimensions
const GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;

// Camera Settings
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Camera with starting position
float lastX = SCR_WIDTH/2.0f, lastY = SCR_HEIGHT/2.0f; // start at center of screen
bool firstMouse = true; // flag for first mouse movement

// Timing Variables
float deltaTime = 0.0f; // Time b/w last frame and current frame
float lastFrame = 0.0f; 

int main()
{
	glfwInit(); //initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	// Specify you will be using OpenGL 4.4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	// Specify you only want to use core OpenGL and throw error at legacy function use
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// specify user should not be able to resize window
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // TODO: Change this to make screen size changeable

	/// CREATE A WINDOW
	// Create window of 800x600 pixels titled LearnOpenGL
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	// If window wasn't properly created, then terminate
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // create the context
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide the cursor when window is in view

	// Set the callback functions for mouse movements
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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

	// Config global opengl state
	glEnable(GL_DEPTH_TEST); // enable the z-buffer and depth testing

	// Build and Compile our shader program
	Shader ourShader("shaders/default.vert", "shaders/default.frag");

	// CREATE RECTANGLE AND RENDER
	// Create a rectangle with normalized (-1 to 1) vertex values
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// World-Space locations of 10 cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// create vertex buffer objects,vertex array objects
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	/// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	/// Bind VBO (copy vertices array into VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//////// Set vertice attributes ///////////

	//Position attribute
	// First vertex position / size of vertex / type of vertex components / if you want to normalize / stride=space b/w vertices/ offset of first component of attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // enables the vertex position attribute
	// Texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // enables the vertex texture attribute

	//////////////////// TEXTURES //////////////////

	// Load and Create the Texture IDs
	// ------------------- texture 1
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture repeated on s & t axes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering when minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // or when magnifying
	// Import the texture from a file
	int tex_width, tex_height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // vertically flip the image so that it is properly aligned
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

	// ------------------- texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture repeated on s & t axes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering when minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // or when magnifying
	// Import the texture from a file
	data = stbi_load("textures/awesomeface.png", &tex_width, &tex_height, &nrChannels, 0); // load from file
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // Gen the texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2 " << std::endl;
	}
	stbi_image_free(data); // Free image memory

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	ourShader.Use(); // remember to activate/use the shader before setting uniforms
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// Loop that will run every frame until something causes termination
	while(!glfwWindowShouldClose(window))
	{
		// Time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Handle inputs
		processInput(window);

		// Rendering Commands go here
		// Clear Colorbuffer  and set background colour to clear to
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer

		// Bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Making sure to activate the Shader
		ourShader.Use();

		// Create a world-to-camera (view) matrix using the camera position, target pos and world up-vector
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view); // Set Uniform

		// Create a camera-to-screen (projection) matrix with perspective projection
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection); // projection matrix actually rarely changes, can only set only once outside loop

		glBindVertexArray(VAO);

		// Create ten translated cubes
		for(unsigned int i =0; i < 10; i++)
		{
			// Create a local-to-world (model) matrix
			glm::mat4 model = glm::mat4(1.0f);//init as identity matrix
			model = glm::translate(model, cubePositions[i]); 
			float angle = 10.0f + 20.0f * i; //add a slight rotation to each cube
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); //rotate with time	
			ourShader.setMat4("model", model); // Set the model uniform

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// Will swap the pointers to the double buffers
		// Checks if any I/O events have been triggered (eg. mouse or keyboard click)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW and clear any resources allocated by it
	glfwTerminate();
	return 0;

}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	// Exits the application if escape was pressed
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Camera movement keys
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Calculate offsets
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

