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


GLuint SCR_WIDTH = 800;
GLuint SCR_HEIGHT = 640;
GLfloat SCR_ASPECT = (float)SCR_HEIGHT / (float)SCR_WIDTH;
const float SPEED_SCALE = 4.0f;
float gameSpeed = SPEED_SCALE;
float rotationSpeed = 1.0f;

// Camera vectors
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Mouse input
bool firstMouse = true;
float pitch = 0;
float yaw = -90;
const float MOUSE_SENSITIVILY = 0.1f;
// Cursour start location
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

void glfwMouseCallback(GLFWwindow* ptrWindow, double xposIn, double yposIn)
{
	float xpos = xposIn;
	float ypos = yposIn;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed coordinats
	lastX = xpos;
	lastY = ypos;

	xoffset *= MOUSE_SENSITIVILY;
	yoffset *= MOUSE_SENSITIVILY;
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tempFront.y = sin(glm::radians(pitch));
	tempFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(tempFront);
}

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
	GLfloat vertex[]
	{
		// positions			// colors			// textures (Reversed coordinates)

		// Front side
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// left top angle
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	// right top angle
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// right bottom angle
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// left bottom angle

		// Back side
		0.5f, 0.5f,	-0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// left top angle
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	// right top angle
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// right bottom angle
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// left bottom angle

		// Right side
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// left top angle
		0.5f, 0.5f,	-0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	// right top angle
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// right bottom angle
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// left bottom angle

		// Left side
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// left top angle
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	// right top angle
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// right bottom angle
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// left bottom angle

		// Down side
		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// left top angle
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	// right top angle
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// right bottom angle
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// left bottom angle


		// Up side
		0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// left top angle
		-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	// right top angle
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// right bottom angle
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f	// left bottom angle
	};
	const GLint indecesSize = sizeof(vertex) / sizeof(GLfloat) / 8 * 1.5;
	const GLint vertexPositionsAmount = sizeof(vertex) / sizeof(GLfloat) / 8;

	GLuint indeces[indecesSize]{};
	// indeces initialization
	for (size_t i = 0, sideStep = 0; i < indecesSize && sideStep < vertexPositionsAmount; i += 6, sideStep += 4)
	{
		indeces[i] = 0 + sideStep;		// First triangle
		indeces[i + 1] = 1 + sideStep;
		indeces[i + 2] = 2 + sideStep;
		indeces[i + 3] = 2 + sideStep;	// Second triangle
		indeces[i + 4] = 3 + sideStep;
		indeces[i + 5] = 0 + sideStep;
	}
	
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

	glfwSetWindowPos(ptrWindow, 150, 150); // Window start position
	glfwMakeContextCurrent(ptrWindow); // Make the window's context current 
	glfwFocusWindow(ptrWindow); // Focus the window as general

	glfwSetInputMode(ptrWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Coursour input
	
	glfwSetCursorPosCallback(ptrWindow, glfwMouseCallback);
	glfwSetWindowSizeCallback(ptrWindow, glfwWindowSizeCallback);
	glfwSetKeyCallback(ptrWindow, glfwWindowKeyCallback);


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

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
	// Vertex buffer
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	// Position points
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

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
	GLint txr_width, txr_height, nrChannels;
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

	stbi_image_free(ptrData); // Deleting texture memory

	/*Loop until the user closes the window */
	glEnable(GL_DEPTH_TEST);
	GLfloat lastFrame = 0;

	while (!glfwWindowShouldClose(ptrWindow))
	{
		float currentFrame = glfwGetTime(); 
		float deltaTime = currentFrame - lastFrame; // Time between frames
		lastFrame = glfwGetTime();

		const float cameraSpeed = gameSpeed * deltaTime; // Moving speed

		// Input
		if (glfwGetKey(ptrWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraPos += cameraFront * cameraSpeed;
		}
		if (glfwGetKey(ptrWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraPos -= cameraFront * cameraSpeed;
		}
		if (glfwGetKey(ptrWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			cameraPos += cameraUp * cameraSpeed;
		}
		if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			cameraPos -= cameraUp * cameraSpeed;
		}
		if (glfwGetKey(ptrWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(ptrWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) gameSpeed = SPEED_SCALE * 3;
		if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) gameSpeed = SPEED_SCALE;
		if (glfwGetKey(ptrWindow, GLFW_KEY_E) == GLFW_PRESS) rotationSpeed += 0.001;
		if (glfwGetKey(ptrWindow, GLFW_KEY_Q) == GLFW_PRESS) rotationSpeed -= 0.01;

		glfwPollEvents(); // Events checking

		// Render here
		glClearColor(96.0f / 255.0f, 69.0f / 255.0f, 107.0f / 255.0f, -1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);

		// Objects coords to map coords
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, float(glfwGetTime()) * glm::radians(45.0f) * rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));
		// Map coords to camera
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// Perspective
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), powf(SCR_ASPECT, -1), 0.1f, 100.0f);

		// Matrix transfer to vertex shader
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

		// Drawing elements
		glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(ptrWindow); // Swap front and back buffers 
	}

	glDeleteBuffers(1, &VAO);
	glfwTerminate();
	return 0;
}