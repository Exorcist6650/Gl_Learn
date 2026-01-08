#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"


#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "resources/stb_image.h"


GLuint SCR_WIDTH = 800;
GLuint SCR_HEIGHT = 640;
GLfloat SCR_ASPECT = (float)SCR_HEIGHT / (float)SCR_WIDTH;
const float SPEED_SCALE = 4.0f;
float gameSpeed = SPEED_SCALE;
float rotationSpeed = 1.0f;

// Camera vectors
glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 3.0f);
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

	//if (pitch > 89.0f) pitch = 89.0f;
	//if (pitch < -89.0f) pitch = -89.0f;

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

int main(int argc, char** argv)
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

	// For resourse manager memory deleting
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

		// Textures path
		std::vector<std::string> texturePaths
		{
			"res/textures/osagePlush.png",
			"res/textures/exa.png",
			"res/textures/osageCry.png",
			"res/textures/osageFear.png",
			"res/textures/osagePlushCap.png",
			"res/textures/osagePlushChristmas.png"
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
			if (glfwGetKey(ptrWindow, GLFW_KEY_E) == GLFW_PRESS) rotationSpeed += 0.01;
			//if (glfwGetKey(ptrWindow, GLFW_KEY_Q) == GLFW_PRESS) rotationSpeed -= 0.01;

			glfwPollEvents(); // Events checking

			// Render here
			glClearColor(96.0f / 255.0f, 69.0f / 255.0f, 107.0f / 255.0f, -1.0f);
			//glClearColor(0, 0, 0, -1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(VAO);
			ptrDefaultShaderProgram->use(); // Using shader program

			// Objects coords to map coords
			glm::mat4 modelCube1 = glm::mat4(1.0f);
			modelCube1 = glm::translate(modelCube1, glm::vec3(1.5f, 0.0f, -3.0f));
			modelCube1 = glm::rotate(modelCube1, float(glfwGetTime()) * glm::radians(45.0f) * rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));

			glm::mat4 modelCube2 = glm::mat4(1.0f);
			modelCube2 = glm::translate(modelCube2, glm::vec3(0.0f, 0.0f, -3.0f));
			modelCube2 = glm::rotate(modelCube2, float(glfwGetTime()) * glm::radians(45.0f) * rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 modelCube3 = glm::mat4(1.0f);
			modelCube3 = glm::translate(modelCube3, glm::vec3(-1.5f, 0.0f, -3.0f));
			modelCube3 = glm::rotate(modelCube3, float(glfwGetTime()) * glm::radians(-45.0f) * rotationSpeed, glm::vec3(-0.5f, 1.0f, 0.0f));

			glm::mat4 modelCube4 = glm::mat4(1.0f);
			modelCube4 = glm::translate(modelCube4, glm::vec3(-0.75f, 1.5f, -3.0f));
			modelCube4 = glm::rotate(modelCube4, float(glfwGetTime()) * glm::radians(-45.0f) * rotationSpeed, glm::vec3(0.5f, 1.0f, 0.0f));

			glm::mat4 modelCube5 = glm::mat4(1.0f);
			modelCube5 = glm::translate(modelCube5, glm::vec3(0.75f, 1.5f, -3.0f));
			modelCube5 = glm::rotate(modelCube5, float(glfwGetTime()) * glm::radians(45.0f) * rotationSpeed, glm::vec3(-0.5f, 1.0f, 0.0f));

			glm::mat4 modelCube6 = glm::mat4(1.0f);
			modelCube6 = glm::translate(modelCube6, glm::vec3(0.0f, 3.0f, -3.0f));
			modelCube6 = glm::rotate(modelCube6, float(glfwGetTime()) * glm::radians(45.0f) * rotationSpeed, glm::vec3(1.0f, 0.0f, 0.0f));


			// Map coords to camera
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			// Perspective
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), powf(SCR_ASPECT, -1), 0.1f, 100.0f);

			// Matrix transfer to vertex shader
			glUniformMatrix4fv(glGetUniformLocation(&ptrDefaultShaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

			// First cube
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelCube1[0][0]);
			// Texture
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, textures.at(5));
			glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 5);
			// Drawing elements
			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

			// Second cube
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelCube2[0][0]);
			// Texture
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, textures.at(4));
			glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 4);
			// Drawing elements
			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

			// Third cube
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelCube3[0][0]);
			// Texture
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, textures.at(3));
			glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 3);
			// Drawing elements
			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

			// Fourth cube
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelCube4[0][0]);
			// Texture
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textures.at(1));
			glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 1);
			// Drawing elements
			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

			// Fifth cube
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelCube5[0][0]);
			// Texture
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, textures.at(2));
			glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 2);
			// Drawing elements
			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

			// Sixth cube
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &modelCube6[0][0]);
			// Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures.at(0));
			glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);
			// Drawing elements
			glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);


			glfwSwapBuffers(ptrWindow); // Swap front and back buffers 
		}
	}

	glDeleteBuffers(1, &VAO);
	glfwTerminate();
	return 0;
}