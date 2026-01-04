#version 460 core

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexColor;
layout (location = 2) in vec2 VertexTexturePos;

out vec3 Color;
out vec2 TexturePosition;

uniform float scrAspect;

void main()
{
	gl_Position = vec4(VertexPos.x, VertexPos.y, VertexPos.z, 1.0f);
	Color = VertexColor;
	TexturePosition = VertexTexturePos;
} 