#pragma once 

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
	Camera(glm::vec3 cameraStartPosition, glm::vec3 cameraFront, glm::vec3 cameraUP, float movingSpeed = 4.0f)
		: camera_pos(cameraStartPosition), camera_front(cameraFront), camera_up(cameraUP), 
		MOVING_SPEED(movingSpeed), speed_scale(movingSpeed) {};
	~Camera() = default;

	void input(GLFWwindow* ptrWindow, GLfloat deltaTime);

	glm::vec3 getPos() const { return camera_pos; }
	glm::vec3 getFront() const { return camera_front; }
	glm::vec3 getUp() const { return camera_up; }

	Camera() = delete;
	Camera(Camera&) = delete;
	Camera& operator=(Camera&) = delete;
private:
	// Camera vectors
	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	
	// Moving parametres
	float MOVING_SPEED;
	float speed_scale;
};