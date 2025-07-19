#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 spriteColor;
uniform sampler2D image;

void main()
{
	FragColor = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}