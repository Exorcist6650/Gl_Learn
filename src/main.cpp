#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Player/camera.h"
#include "CoreTypes.h"


#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "resources/stb_image.h"

// State of window
WindowState winState(800, 640);

Camera playerCam(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), &winState, 6.0f);

void glfwMouseCallback(GLFWwindow* ptrWindow, double xposIn, double yposIn)
{
	playerCam.MouseCallback(ptrWindow, xposIn, yposIn);
}

void glfwWindowSizeCallback(GLFWwindow* ptrWindow, GLint width, GLint height) {
	winState.SCR_WIDTH = width;
	winState.SCR_HEIGHT = height;
	winState.SCR_ASPECT = (float)winState.SCR_HEIGHT / (float)winState.SCR_WIDTH;

	glViewport(0, 0, winState.SCR_WIDTH, winState.SCR_HEIGHT);
}

void glfwWindowKeyCallback(GLFWwindow* ptrWindow, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(ptrWindow, GLFW_TRUE);
}

int main(int argc, char** argv)
{
	GLfloat vertexCube[]
	{
		// positions			// colors			// textures (reversed coordinates)

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
	const GLint indecesSize = sizeof(vertexCube) / sizeof(GLfloat) / 8 * 1.5;
	const GLint vertexPositionsAmount = sizeof(vertexCube) / sizeof(GLfloat) / 8;

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
	GLfloat vertexPlatform[]
	{
		// positons				// colors			// texture (reversed coordinates)
		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,	// right top angle
		-1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	// left top angle
		1.0f, 0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,	// right bottom angle
		-1.0f, 0.0f, -1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// left bottom angle	
	};

	GLFWwindow* ptrWindow;

	// Initialize glfw
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
	ptrWindow = glfwCreateWindow(winState.SCR_WIDTH, winState.SCR_HEIGHT, "OsageEngine", NULL, NULL);
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
	//std::cout << "SCR_WIDTH: " << SCR_WIDTH << "px\n" << "SCR_HEIGHT: " << SCR_HEIGHT << "px\n" << "SCR_ASPECT: " << SCR_ASPECT << std::endl;


	// For resourse manager smart pointer deleting
	{
		// Resource manager enable
		ResourceManager resourseManager(argv[0]);
		// Shader program creating
		std::shared_ptr<Renderer::ShaderProgram> ptrDefaultShaderProgram =
			resourseManager.loadShaders("DefaultShader", "res/shaders/shader.vert", "res/shaders/shader.frag");
		if (!ptrDefaultShaderProgram)
		{
			std::cerr << "ERROR::SHADER::PROGRAM::CREATION_FAILED" << std::endl;
			return -1;
		}

		// Vertex cube array
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCube), vertexCube, GL_STATIC_DRAW);
		// Position points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// Texture position
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// Vertex platform array
		GLuint VAO_platform;
		glGenVertexArrays(1, &VAO_platform);
		glBindVertexArray(VAO_platform);

		// Vertex buffer
		GLuint VBO_platform;
		glGenBuffers(1, &VBO_platform);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_platform);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPlatform), vertexPlatform, GL_STATIC_DRAW);
		// Positions points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
		// Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// Texture position
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		// Textures path
		std::vector<std::string> texturePaths
		{
			"res/textures/osagePlush.png",
			"res/textures/floor_texture.png"
		};

		std::vector <unsigned int> textures(6);
		// Textures
		for (size_t i = 0; i < texturePaths.size(); i++)
		{
			GLint txr_width, txr_height, nrChannels;
			unsigned char* ptrData = stbi_load(texturePaths.at(i).c_str(), &txr_width, &txr_height, &nrChannels, 0);
			glGenTextures(1, &textures.at(i));
			glBindTexture(GL_TEXTURE_2D, textures.at(i));
			if (ptrData)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txr_width, txr_height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptrData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else std::cout << "FAILED::TEXTURE::LOAD" << std::endl;

			stbi_image_free(ptrData); // Deleting texture memory
		}

		/*Loop until the user closes the window */
		glEnable(GL_DEPTH_TEST);
		GLfloat lastFrame = 0;

		float lightCubeX = 5.0f;
		float lightCubeY = 5.0f;
		float lightCubeZ = -3.0f;

		while (!glfwWindowShouldClose(ptrWindow))
		{
			float currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame; // Time between frames
			lastFrame = glfwGetTime();


			if (glfwGetKey(ptrWindow, GLFW_KEY_UP) == GLFW_PRESS)
			{
				lightCubeZ -= 0.01f;
			}
			if (glfwGetKey(ptrWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				lightCubeZ += 0.01f;
			}
			if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				lightCubeX -= 0.01f;
			}
			if (glfwGetKey(ptrWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				lightCubeX += 0.01f;
			}
			if (glfwGetKey(ptrWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			{
				lightCubeY += 0.01f;
			}
			if (glfwGetKey(ptrWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			{
				lightCubeY -= 0.01f;
			}

			// Input
			playerCam.input(ptrWindow, deltaTime);

			glfwPollEvents(); // Events checking

			// Render here
			glClearColor(96.0f / 255.0f, 69.0f / 255.0f, 107.0f / 255.0f, -1.0f);
			//glClearColor(0, 0, 0, -1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Cube array
			glBindVertexArray(VAO);
			ptrDefaultShaderProgram->use(); // Using shader program

			// Objects coords to map coords
			glm::mat4 modelCube = glm::mat4(1.0f);
			modelCube = glm::translate(modelCube, glm::vec3(0.0f, 0.5f, -3.0f));
			//modelCube = glm::rotate(modelCube, float(glfwGetTime()) * glm::radians(45.0f) * rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));

			// Map coords to camera
			glm::mat4 view = glm::lookAt(playerCam.getPos(), playerCam.getPos() + playerCam.getFront(), playerCam.getUp());
			// Perspective
			glm::mat4 projection = glm::perspective(glm::radians(90.0f), powf(winState.SCR_ASPECT, -1), 0.1f, 100.0f);

			// Matrix transfer to vertex shader
			glUniformMatrix4fv(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "projection"), 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "model"), 1, GL_FALSE, &modelCube[0][0]);
			glUniform1i(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "useTexture"), false);

			// Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures.at(0));
			glUniform1i(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "textureSampler"), 0);

			// Drawing elements cube
			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

			// Light cude 
			glm::mat4 modelLightCube = glm::mat4(1.0f);
			modelLightCube = glm::translate(modelLightCube, glm::vec3(lightCubeX, lightCubeY, lightCubeZ));
			modelLightCube = glm::scale(modelLightCube, glm::vec3(0.5f, 0.5f, 0.5f));

			glUniformMatrix4fv(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "model"), 1, GL_FALSE, &modelLightCube[0][0]);
			glUniform1i(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "useTexture"), true);
			
			glBindTexture(GL_TEXTURE_2D, 0); // No texture

			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

			// Platform array
			glBindVertexArray(VAO_platform);

			glm::mat4 modelPlatform = glm::mat4(1.0f);
			modelPlatform = glm::translate(modelPlatform, glm::vec3(0.0f, 0.0f, -3.0f));
			modelPlatform = glm::scale(modelPlatform, glm::vec3(5.0f, 1.0f, 5.0f));

			// New model transfer to vertex shader
			glUniformMatrix4fv(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "model"), 1, GL_FALSE, &modelPlatform[0][0]);
			glUniform1i(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "useTexture"), false);

			// Texture
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures.at(1));
			glUniform1i(glGetUniformLocation(resourseManager.getShaderProgram("DefaultShader")->getProgramID(), "textureSampler"), 1);

			// Platform drawing
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glfwSwapBuffers(ptrWindow); // Swap front and back buffers 
		}
	}
	glfwTerminate();
	return 0;
}