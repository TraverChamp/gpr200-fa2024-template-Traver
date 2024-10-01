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
	// load and create a texture 
   // -------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned int texture1 = loadTexture("assets/textures/redgem.png", GL_RGBA, GL_NEAREST, GL_REPEAT);
	unsigned int texture2 = loadTexture("assets/textures/whitewebb.png", GL_RGBA, GL_NEAREST, GL_REPEAT);
	Shader mine("..\\..\\..\\..\\assignments\\Assignment2_Indicies\\assets\\Shader.vert", "..\\..\\..\\..\\assignments\\Assignment2_Indicies\\assets\\Shader.frag");
	glUniform1i(glGetUniformLocation(mine.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(mine.ID, "texture2"), 1);
	//second shader
	Shader theirs("..\\..\\..\\..\\assignments\\Assignment2_Indicies\\assets\\bgShader.vert", "..\\..\\..\\..\\assignments\\Assignment2_Indicies\\assets\\bgShader.frag");
	unsigned int texture3 = loadTexture("assets/textures/bluebriccs.png", GL_RGBA,GL_NEAREST, GL_CLAMP_TO_EDGE);
	// or set it via the texture class
	mine.setInt("texture1", 0);
	mine.setInt("texture2", 1);
	theirs.setInt("texture1", 2);
	stbi_set_flip_vertically_on_load(true);
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); 
			float time = (float)glfwGetTime();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		theirs.use();
		mine.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		mine.setFloat("_Time", time);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	printf("Shutting down...");
}
