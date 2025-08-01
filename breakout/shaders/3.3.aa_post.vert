#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform bool  shake;
uniform float time;

void main()
{
    TexCoords = aTexCoords;
    float strength = 0.01;
    gl_Position = vec4(aPos, 0.0, 1.0);
    if (shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
} 