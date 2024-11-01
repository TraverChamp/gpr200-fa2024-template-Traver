#include <stdio.h>
#include <math.h>
#include <ew/external/stb_image.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <TraverB/shader.h>
#include <fstream>
#include <sstream>
#include "TraverB/texture.h"
#include "load_asset.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
using namespace std;
const int SCREEN_WIDTH = 2080;
const int SCREEN_HEIGHT = 1440;
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 60.0f;


float vertices[] = {
	   -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,

	   -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,  1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
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
	   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f
};
unsigned int indicies[] = {
	0, 1, 2,
	2, 3, 0
};
struct Material {
	float ambientK; //Ambient coefficient (0-1)							  
	float diffuseK; //Diffuse coefficient (0-1)							  
	float specular; //Specular coefficient (0-1)						  
	float shininess; //Shininess										  
};
// world space positions of our cubes
glm::vec3 cubePositions[] = {
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(3.0f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f),
	glm::vec3(-2.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  -5.0f, 15.0f),
	glm::vec3(-1.5f, 2.2f, 2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(3.6f, -0.4f, 7.0f),
	glm::vec3(-3.4f,  6.0f, -7.5f),
	glm::vec3(2.6f, -4.0f, -5.0f),
	glm::vec3(1.5f,  4.0f, 5.0f),
	glm::vec3(-3.0f,  0.4f, -3.0f),
	glm::vec3(-2.6f,  -0.5f, -1.5f),
	glm::vec3(-5.6f,  -1.5f, -3.5f)
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
	// Resolve asset paths.
	auto texture1Path = GetAssetPath("textures/redgem.png");
	auto texture2Path = GetAssetPath("textures/whitewebb.png");
	auto texture3Path = GetAssetPath("textures/bluebriccs.png");
	auto shaderVPath = GetAssetPath("Shader.vert");
	auto shaderFPath = GetAssetPath("Shader.frag");
	auto lShaderVPath = GetAssetPath("lit.vert");
	auto lShaderFPath = GetAssetPath("lit.frag");
	auto uShaderVPath = GetAssetPath("unlit.vert");
	auto uShaderFPath = GetAssetPath("unlit.frag");
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
	Shader lightCubeShader(lShaderVPath.c_str(), lShaderFPath.c_str());
	Shader unlitShader(uShaderVPath.c_str(), uShaderFPath.c_str());
	//Shader backgroundShader(shaderBackgroundVPath.c_str(), shaderBackgroundFPath.c_str());
	//material setup
	Material mat;
	mat.shininess = 50.0f;
	mat.ambientK = 0.1f;
	mat.diffuseK = 0.5f;
	mat.specular = 1.0f;
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 unlitColor = glm::mat4(1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f);
	// or set it via the texture class
	//IMGUI stuffs
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	stbi_set_flip_vertically_on_load(true);
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); 
		deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();
		processInput(window);
		

		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// Bind the current texture.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// Bind the samples to texture units.
		glUniform1i(glGetUniformLocation(lightCubeShader.ID, "texture1"), 0);
		glUniform1i(glGetUniformLocation(lightCubeShader.ID, "texture2"), 1);
		
		
		// Draw quad
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
			lightCubeShader.use();
			// pass projection matrix to shader (note that in this case it could change every frame)
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			lightCubeShader.setMat4("projection", projection);

			// camera/view transformation
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			//glm::mat4 view = glm::lookAt(glm::vec3(0,0,-5), glm::vec3(0),glm::vec3(0,1,0));
			lightCubeShader.setMat4("view", view);

		
			// Update current time in the bound shader program.
			lightCubeShader.setFloat("_Time", (float)glfwGetTime());
			for (unsigned int i = 0; i < 19; i++)
			{
				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				lightCubeShader.setMat4("model", model);
				lightCubeShader.setVec3("lightPos", lightPos);
				lightCubeShader.setFloat("shininessStrength", mat.shininess);
				lightCubeShader.setFloat("ambientStrength", mat.ambientK);
				lightCubeShader.setFloat("specularStrength", mat.specular);
				lightCubeShader.setFloat("diffuse", mat.diffuseK);
				lightCubeShader.setVec3("lightColor", lightColor);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			bool test = false;
			// world transformation
			glm::mat4 model = glm::mat4(1.0f);
			//Imgui drawing
			{
				//window establish
				ImGui_ImplGlfw_NewFrame();
				ImGui_ImplOpenGL3_NewFrame();
				ImGui::NewFrame();

				//variable implementation
				ImGui::Begin("Settings");
				ImGui::Text("Add controlss Here!");
				ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
				ImGui::ColorEdit3("Light Color", &lightColor.r); //effects the textured bricks, not the white one in the center projecting light
				ImGui::SliderFloat("Ambient K", &mat.ambientK, 0.0f, 1.0f);
				ImGui::SliderFloat("Shininess", &mat.shininess, 2.0f, 1024.0f);
				ImGui::SliderFloat("Specular K", &mat.specular, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse K", &mat.diffuseK, 0.0f, 1.0f);
				ImGui::Checkbox("Check", &test);
				ImGui::End();

				//render all
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			
			

			// also draw the lamp object
			unlitShader.use();
			unlitShader.setMat4("view", view);
			unlitShader.setMat4("projection", projection);
			unlitShader.setMat4("_Color", glm::mat4(1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, lightPos);
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(0.2f));
			unlitShader.setMat4("model", model);

			glBindVertexArray(lightCubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//update swapchain
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);
	printf("Shutting down...");

}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ==GLFW_PRESS) {
		float cameraSpeed = static_cast<float>(5 * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		//up and down
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		//up and down
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //Unlocks
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Locks
	}
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
	if (fov > 120.0f)
		fov = 120.0f;
}
