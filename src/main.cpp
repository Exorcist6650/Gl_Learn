#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

GLfloat vertex[]
{
	// positions        // colors
	-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f, // left up angle
	0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // right up angle
	-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,  // left down angle
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // right down angle
};

GLuint SCR_WIDTH = 640;
GLuint SCR_HEIGHT = 480;
GLfloat SCR_ASPECT = (float)SCR_WIDTH / (float)SCR_HEIGHT;


void glfwWindowSizeCallback(GLFWwindow* ptrWindow, GLint width, GLint height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	SCR_ASPECT = (float)SCR_WIDTH / (float)SCR_HEIGHT;

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	std::cout << "SCR_WIDTH: " << SCR_WIDTH << "px\n" << "SCR_HEIGHT: " << SCR_HEIGHT << "px\n" << "SCR_ASPECT: " << SCR_ASPECT << std::endl;
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

	/* Make the window's context current */
	glfwMakeContextCurrent(ptrWindow);
	 
	if (!gladLoadGL()) std::cout << "ERROR::GLAD::OPEN_FAILED" << std::endl; // Glad exception

	// Device info
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout  << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
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
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(ptrWindow))
	{
		/* Render here */
		glClearColor(1.0f, 1.0f, 1.0f, -1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);

		// Location configuration by screen ratio
		int uni_aspect = glGetUniformLocation(shaderProgram, "scrAspect");
		glUniform1f(uni_aspect, (float)SCR_ASPECT);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(ptrWindow); // Swap front and back buffers 
		glfwPollEvents(); // Events checking
	}

	glfwTerminate();
	return 0;
}