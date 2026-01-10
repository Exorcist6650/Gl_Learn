#version 460 core

in vec3 Color;
in vec2 TexturePosition;

out vec4 FragColor;
uniform sampler2D textureSampler;
uniform bool useTexture;

void main()
{
	if (!useTexture) FragColor = texture(textureSampler, TexturePosition);
	else FragColor = vec4(Color, 1.0f);
}