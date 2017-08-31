#include <glad/glad.h>
//GLFW
#include <GLFW/glfw3.h>
//OTHERS
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void setShaderLightsUniforms(Shader &containerShader, glm::vec3 *pointLightPositions);

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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// CREATE A WINDOW
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // create the context
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide the cursor when window is in view

	// Set the callback functions for mouse movements and frame size change
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Config global opengl state
	glEnable(GL_DEPTH_TEST); // enable the z-buffer and depth testing

	 // World-Space locations of 4 lamps
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// Build and Compile our shader program
	Shader ourShader("shaders/model_loading.vert", "shaders/model_loading.frag");
	Shader lampShader("shaders/light.vert", "shaders/light.frag");

	// Load models
	Model ourModel("models/nanosuit.obj");

	// Loop that will run every frame until something causes termination
	while(!glfwWindowShouldClose(window))
	{
		// per-frame Time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Handle inputs
		processInput(window);

		// Render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Colorbuffer and clear the depth buffer

		// enable the shader before setting uniforms
		ourShader.Use();

		// Create a world-to-camera (view) matrix using the camera position, target pos and world up-vector
		glm::mat4 view = camera.GetViewMatrix();
		// Create a camera-to-screen (projection) matrix with perspective projection
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		// Render the laoded model
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // it's a bit too big for the scene, so scale down
		ourShader.setMat4("model", model);
		setShaderLightsUniforms(ourShader, pointLightPositions);
		ourModel.Draw(ourShader);

		// Create the lamps
		lampShader.Use();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]); // place at world position
			model = glm::scale(model, glm::vec3(0.2f)); // scale down the lamp
			lampShader.setMat4("model", model); // set light uniforms
			ourModel.Draw(ourShader);
		}

		// Will swap the pointers to the double buffers
		// Checks if any I/O events have been triggered (eg. mouse or keyboard click)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{    
	// make sure the viewport matches the new window dimensions; note that width and 
	 // height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

/// Set up the uniforms of the directional light, 4 point lights and one spotlight for the shader
void setShaderLightsUniforms(Shader &containerShader, glm::vec3 *pointLightPositions)
{
	containerShader.setFloat("material.shininess", 32.0f);
	containerShader.setVec3("viewPos", camera.Position);

	// Directional Light
	containerShader.setVec3("DirLight.direction", -0.2f, -1.0f, -0.3f);
	containerShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	containerShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	containerShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	containerShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	containerShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	containerShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	containerShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	containerShader.setFloat("pointLights[0].constant", 1.0f);
	containerShader.setFloat("pointLights[0].linear", 0.09);
	containerShader.setFloat("pointLights[0].quadratic", 0.032);
	// point light 2
	containerShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	containerShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	containerShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	containerShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	containerShader.setFloat("pointLights[1].constant", 1.0f);
	containerShader.setFloat("pointLights[1].linear", 0.09);
	containerShader.setFloat("pointLights[1].quadratic", 0.032);
	// point light 3
	containerShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	containerShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	containerShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	containerShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	containerShader.setFloat("pointLights[2].constant", 1.0f);
	containerShader.setFloat("pointLights[2].linear", 0.09);
	containerShader.setFloat("pointLights[2].quadratic", 0.032);
	// point light 4
	containerShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	containerShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	containerShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	containerShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	containerShader.setFloat("pointLights[3].constant", 1.0f);
	containerShader.setFloat("pointLights[3].linear", 0.09);
	containerShader.setFloat("pointLights[3].quadratic", 0.032);
	// spotLight
	containerShader.setVec3("spotLight.position", camera.Position);
	containerShader.setVec3("spotLight.direction", camera.Front);
	containerShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	containerShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	containerShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	containerShader.setFloat("spotLight.constant", 1.0f);
	containerShader.setFloat("spotLight.linear", 0.09);
	containerShader.setFloat("spotLight.quadratic", 0.032);
	containerShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	containerShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}
