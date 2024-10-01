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

float vertices[] = {
	//positions are first 3, colors are next 4, then last 2 are texture coords
	-0.5f, -0.5f, 0.0f, 0.75f, 0.0f, 0.75f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
};
unsigned int indicies[] = {
	0, 1, 2,
	2, 3, 0
};
float texCoords[] = {
	0.5f, -0.5f,  // lower-left corner  
	0.0f, 1.0f,  // lower-right corner
	-0.5f, -1.0f   // top-center corner
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
	//Initialization goes here!
	//Vertex data
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Vertex data goes here
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	unsigned int EBO;
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
	// Create shader program for draing textured quad.
	Shader texturedShader(shaderVPath.c_str(), shaderFPath.c_str());
	Shader backgroundShader(shaderBackgroundVPath.c_str(), shaderBackgroundFPath.c_str());
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
		// Draw #0
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
		}
		// Draw #1 
		{
			// Binds the first shader to the pipeline.
			texturedShader.use();

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
	
