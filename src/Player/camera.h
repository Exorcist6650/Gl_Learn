#pragma once 

#include <glm/glm.hpp>
#include "../CoreTypes.h"

class Camera {
public:
	Camera(glm::vec3 cameraStartPosition, glm::vec3 cameraFront, glm::vec3 cameraUP, WindowState* ptrWinState, 
		float movingSpeed = 4.0f, float mouseSensitivily = 0.1f) : 
		// Vectors
		camera_pos(cameraStartPosition), camera_front(cameraFront), camera_up(cameraUP), 
		// Window
		ptr_win_state(ptrWinState), lastX(ptrWinState->SCR_WIDTH / 2), lastY(ptrWinState->SCR_HEIGHT / 2), MOUSE_SENSITIVILY(mouseSensitivily), 
		// Move logic
		MOVING_SPEED(movingSpeed), speed_scale(movingSpeed) {}

	~Camera() = default;

	void input(GLFWwindow* ptrWindow, GLfloat deltaTime);
	void MouseCallback(GLFWwindow* ptrWindow, double xposIn, double yposIn);

	glm::vec3 getPos() const { return camera_pos; }
	glm::vec3 getFront() const { return camera_front; }
	glm::vec3 getUp() const { return camera_up; }

	void setPos(glm::vec3 cameraPos) { camera_pos = cameraPos; }
	void setFront(glm::vec3 cameraFront) { camera_front = cameraFront; }
	void setUp(glm::vec3 cameraUP) { camera_up = cameraUP; }

	Camera() = delete;
	Camera(Camera&) = delete;
	Camera& operator=(Camera&) = delete;
private:
	WindowState* ptr_win_state;

	// Camera vectors
	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	
	// Moving parametres
	float MOVING_SPEED;
	float speed_scale;

	// Mouse input
	const float MOUSE_SENSITIVILY;
	float lastX;
	float lastY;

	// Const
	bool firstMouse = true;
	float pitch = 0;
	float yaw = -90;

};