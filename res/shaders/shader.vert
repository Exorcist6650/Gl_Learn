#version 460 core

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

uniform float scr_aspect;
uniform float cosTheta;
uniform float sinTheta;

void main()
{
	mat2 rotation = mat2(
		cosTheta, -sinTheta,
		sinTheta, cosTheta
	);

	vec2 rotatedPos = rotation * VertexPos.xy;

	vec4 newPosition;
	newPosition.x = rotatedPos.x * scr_aspect;	
	newPosition.y = rotatedPos.y;
	newPosition.z = VertexPos.z;
	newPosition.w = 1.0;
	gl_Position = newPosition;
	Color = VertexColor;
} 