#version 460 core

in vec3 Color;
in vec2 TexturePosition;

out vec4 FragColor;

uniform sampler2D textureSampler;
void main()
{
	FragColor = texture(textureSampler, TexturePosition);
}