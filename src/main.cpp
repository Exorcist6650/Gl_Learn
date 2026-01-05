#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "resources/stb_image.h"

GLfloat vertex[]
{
	// positions			// colors	

	// Front side
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left top angle
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right top angle
	0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle

	0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle
	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left bottom angle
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left top angle

	// Back side
	0.5f, 0.5f,	-0.5f,		1.0f, 1.0f, 1.0f,	// left top angle
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// right top angle
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	// right bottom angle

	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	// right bottom angle
	0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// left bottom angle
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// left top angle

	// Right side
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left top angle
	0.5f, 0.5f,	-0.5f,		1.0f, 1.0f, 1.0f,	// right top angle
	0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle

	0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle
	0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left bottom angle
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left top angle

	// Left side
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// left top angle
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right top angle
	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle

	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	// left bottom angle
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// left top angle

	// Down side
	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left top angle
	0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right top angle
	0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle

	0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	// left bottom angle
	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left top angle

	// Up side
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// left top angle
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	// right top angle
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle

	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// right bottom angle
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	// left bottom angle
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f	// left top angle

};


GLuint SCR_WIDTH = 800;
GLuint SCR_HEIGHT = 640;
GLfloat SCR_ASPECT = (float)SCR_HEIGHT / (float)SCR_WIDTH;


void glfwWindowSizeCallback(GLFWwindow* ptrWindow, GLint width, GLint height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	SCR_ASPECT = (float)SCR_HEIGHT / (float)SCR_WIDTH;

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	std::cout << "SCR_WIDTH: " << SCR_WIDTH << "px\n" << "SCR_HEIGHT: " << SCR_HEIGHT << "px\n" << "SCR_ASPECT: " << SCR_ASPECT << std::endl;
}

void glfwWindowKeyCallback(GLFWwindow* ptrWindow, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(ptrWindow, GLFW_TRUE);
}

int main(void)
{
	
	GLFWwindow* ptrWindow;

	// Initialize the library
	if (!glfwInit())
	{
		std::cout << "ERROR::GLFW::INIT_FAILED" << std::endl;
		return -1;
	}

	// Opengl version setting
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	ptrWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Square", NULL, NULL);
	if (!ptrWindow)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(ptrWindow, glfwWindowSizeCallback);
	glfwSetKeyCallback(ptrWindow, glfwWindowKeyCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(ptrWindow);

	if (!gladLoadGL()) std::cout << "ERROR::GLAD::OPEN_FAILED" << std::endl; // Glad exception

	// Device info
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << std::filesystem::current_path().string() << std::endl;
	std::cout << "SCR_WIDTH: " << SCR_WIDTH << "px\n" << "SCR_HEIGHT: " << SCR_HEIGHT << "px\n" << "SCR_ASPECT: " << SCR_ASPECT << std::endl;

	// Vertex array
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex buffer
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	// Position points
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture position
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);*/

	// Shaders read
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// Exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open("res/shaders/shader.vert");
		fShaderFile.open("res/shaders/shader.frag");

		std::stringstream vShaderStream, fShaderStream; // String stream

		// Read files buffers into string streams
		vShaderStream << vShaderFile.rdbuf(); // ifstream::rdbuf() transfer all file (not getline reading)
		fShaderStream << fShaderFile.rdbuf();

		// Closing file handlers
		vShaderFile.close();
		fShaderFile.close();

		// Stream to string convertation
		vertexCode = vShaderStream.str(); // From thread to string
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure ex)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str(); // C str convertation for OpenGL
	const char* fShaderCode = fragmentCode.c_str();

	char infoLog[512]; // For exceptions
	GLint success;

	// Vertex shader compiling
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	// Vertex compiling errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader compiling
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	// Fragment compiling errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Program linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Shader deleting
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Texture
	/*GLint txr_width, txr_height, nrChannels;
	unsigned char* ptrData = stbi_load("res/textures/osagePlush.png", &txr_width, &txr_height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (ptrData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txr_width, txr_height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptrData);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else std::cout << "FAILED::TEXTURE::LOAD" << std::endl;
	stbi_image_free(ptrData);*/

	/* Loop until the user closes the window */
	//std::cout << points[sizeof(points) / sizeof(GLfloat) - 6];

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(ptrWindow))
	{
		glfwPollEvents(); // Events checking

		// Render here
		glClearColor(96.0f / 255.0f, 69.0f / 255.0f, 107.0f / 255.0f, -1.0f);
		//glClearColor(1, 1, 1, -1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);

		// Objects coords to map coords
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, float(glfwGetTime()) * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		// Map coords to camera
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		// Perspective
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), powf(SCR_ASPECT, -1), 0.1f, 100.0f);

		// Matrix transfer to vertex shader
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

		// Drawing elements
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(ptrWindow); // Swap front and back buffers 
	}

	glDeleteBuffers(1, &VAO);
	glfwTerminate();
	return 0;
}