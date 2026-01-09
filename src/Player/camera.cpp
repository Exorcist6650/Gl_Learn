#include "camera.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Camera::input(GLFWwindow* ptrWindow, GLfloat deltaTime)
{
	if (glfwGetKey(ptrWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera_pos += camera_front * speed_scale * deltaTime;
		camera_pos.y = 0.5f;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera_pos -= camera_front * speed_scale * deltaTime;
		camera_pos.y = 0.5f;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera_pos += camera_up * speed_scale * deltaTime;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera_pos -= camera_up * speed_scale * deltaTime;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * speed_scale * deltaTime;
		camera_pos.y = 0.5f;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * speed_scale * deltaTime;
		camera_pos.y = 0.5f;
	}
	if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) speed_scale = MOVING_SPEED * 3.0f;
	if (glfwGetKey(ptrWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) speed_scale = MOVING_SPEED;
}
