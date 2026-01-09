#include "camera.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Camera::input(GLFWwindow* ptrWindow, GLfloat deltaTime)
{
	float tempPosY = camera_pos.y;
	glm::vec3 horizFront = glm::normalize(glm::vec3(camera_front.x, 0.0f, camera_front.z));

	if (glfwGetKey(ptrWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_pos += horizFront * speed_scale * deltaTime;
		//camera_pos.y = tempPosY;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_pos -= horizFront * speed_scale * deltaTime;
		//camera_pos.y = tempPosY;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera_pos += camera_up * speed_scale * deltaTime;
		tempPosY = camera_pos.y;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera_pos -= camera_up * speed_scale * deltaTime;
		tempPosY = camera_pos.y;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * speed_scale * deltaTime;
		camera_pos.y = tempPosY;

	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * speed_scale * deltaTime;
		camera_pos.y = tempPosY;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) speed_scale = MOVING_SPEED * 3.0f;
	if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) speed_scale = MOVING_SPEED;
}

void Camera::MouseCallback(GLFWwindow* ptrWindow, double xposIn, double yposIn)
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
	setFront(glm::normalize(tempFront));
}
