#include <stdio.h>
#include <math.h>
#include <ew/external/stb_image.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <TraverB/shader.h>
#include <fstream>
#include <sstream>
#include "TraverB/texture.h"
#include "load_asset.h"
using namespace std;
const int SCREEN_WIDTH = 2080;
const int SCREEN_HEIGHT = 1440;
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float vertices[] = {
	   -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,

	   -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f
};
unsigned int indicies[] = {
	0, 1, 2,
	2, 3, 0
};
// world space positions of our cubes
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
int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	glEnable(GL_DEPTH_TEST);
	//Initialization goes here!
	//Vertex data
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	//Vertex data goes here
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Resolve asset paths.
	auto texture1Path = GetAssetPath("textures/redgem.png");
	auto texture2Path = GetAssetPath("textures/whitewebb.png");
	auto texture3Path = GetAssetPath("textures/bluebriccs.png");
	auto shaderVPath = GetAssetPath("Shader.vert");
	auto shaderFPath = GetAssetPath("Shader.frag");
	auto shaderBackgroundVPath = GetAssetPath("bgShader.vert");
	auto shaderBackgroundFPath = GetAssetPath("bgShader.frag");
	// load and create a texture 
   // -------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned int texture1 = loadTexture(texture1Path.c_str(), GL_RGBA, GL_NEAREST, GL_REPEAT);
	unsigned int texture2 = loadTexture(texture2Path.c_str(), GL_RGBA, GL_NEAREST, GL_REPEAT);
	unsigned int texture3 = loadTexture(texture3Path.c_str(), GL_RGBA, GL_NEAREST, GL_CLAMP_TO_EDGE);
	// Create shader program for drawing textured quad.
	Shader texturedShader(shaderVPath.c_str(), shaderFPath.c_str());
	//Shader backgroundShader(shaderBackgroundVPath.c_str(), shaderBackgroundFPath.c_str());
	//second shader
	
	// or set it via the texture class
	
	stbi_set_flip_vertically_on_load(true);
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); 
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		/* Draw #0
		{
			// Binds the first shader to the pipeline.
			backgroundShader.use();

			// Update current time in the bound shader program.
			backgroundShader.setFloat("_Time", (float)glfwGetTime());
			// Bind the current texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture3);
			// Bind the samples to texture units.
			glUniform1i(glGetUniformLocation(texturedShader.ID, "texture1"), 0);
			// Draw quad
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}  */
		// Draw #1 
		{
			// Binds the first shader to the pipeline.
			texturedShader.use();
			// pass projection matrix to shader (note that in this case it could change every frame)
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			texturedShader.setMat4("projection", projection);

			// camera/view transformation
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		    texturedShader.setMat4("view", view);

		
			// Update current time in the bound shader program.
			texturedShader.setFloat("_Time", (float)glfwGetTime());
			// Bind the current texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);
			// Bind the samples to texture units.
			glUniform1i(glGetUniformLocation(texturedShader.ID, "texture1"), 0);
			glUniform1i(glGetUniformLocation(texturedShader.ID, "texture2"), 1);
			// Draw quad
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		//update swapchain
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	printf("Shutting down...");
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
