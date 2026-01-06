#version 460 core

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexColor;
layout (location = 2) in vec2 VertexTexturePos;

out vec3 Color;
out vec2 TexturePosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(VertexPos, 1.0f);
	Color = VertexColor;
	TexturePosition = VertexTexturePos;
} 