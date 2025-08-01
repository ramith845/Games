#version 330 core

layout (location=0) in vec4 vPos;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 projection;
uniform vec2 size;
uniform vec2 offset;
uniform vec4 color;

void main()
{
	TexCoords = vPos.zw;
	ParticleColor = color;
	gl_Position = projection * model * vec4(vPos.xy * size + offset, 0.0, 1.0);
}