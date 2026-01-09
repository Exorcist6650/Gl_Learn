#pragma once
#include <GLFW/glfw3.h>

struct WindowState
{
	WindowState(GLuint width, GLuint height) : SCR_WIDTH(width), SCR_HEIGHT(height), SCR_ASPECT((float)height / (float)width) {}
	WindowState() = delete;

	GLuint SCR_WIDTH;
	GLuint SCR_HEIGHT;
	GLfloat SCR_ASPECT;
};