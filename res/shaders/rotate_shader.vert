#version 460 core

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

uniform float scrAspect;
uniform float cosTheta;
uniform float sinTheta;

void main()
{
	mat2 rotation = mat2(
		cosTheta, -sinTheta,
		sinTheta, cosTheta
	);
	vec2 rotatedPos = rotation * VertexPos.xy;

	float xScrAspect = rotatedPos.x * scrAspect;
	if (xScrAspect > 1.0f) xScrAspect = 1.0f; 
	if (xScrAspect < -1.0f) xScrAspect = -1.0f;  

	gl_Position = vec4(xScrAspect, rotatedPos.y, VertexPos.z, 1.0);
	Color = VertexColor;
} 