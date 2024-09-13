#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 2080;
const int SCREEN_HEIGHT = 1440;
float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
	 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
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

	const char* vertexShaderSource = R"(#version 330 core
		layout(location = 0) in vec3 aPos;
		layout (location = 1) in vec4 aColor;
		out vec4 Color;
		uniform float _Time;
		void main()
		{
		   Color=aColor;
			vec3 pos= aPos;
			pos.y+=(cos(_Time*1.5+(pos.x*0.5)+(pos.z*0.75))*0.5);
		   gl_Position = vec4(aPos.x, pos.y, aPos.z, 1.0);
})";
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success{};
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	char infoLog[512];
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	const char* fragmentShaderSource = R"(#version 330 core
out vec4 FragColor;
in vec4 Color;
uniform float _Time;
void main()
{
    FragColor = Color * abs(tan(_Time));
})";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "Error:Shader::Fragment:Compilation failed" << infoLog << endl;
	}
	//created and compiled fragment shader by here

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "Error:shaderProgram::Compilation failed" << infoLog << endl;
	}
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); 
			float time = (float)glfwGetTime();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		int timeLoc = glGetUniformLocation(shaderProgram, "_Time");
		glUniform1f(timeLoc, time);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
	printf("Shutting down...");
}
