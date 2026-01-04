#version 460 core

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexColor;
layout (location = 2) in vec2 VertexTexturePos;

out vec3 Color;
out vec2 TexturePosition;

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

	gl_Position = vec4(rotatedPos.x, rotatedPos.y, VertexPos.z, 1.0);
	Color = VertexColor;
	TexturePosition = VertexTexturePos;
} 