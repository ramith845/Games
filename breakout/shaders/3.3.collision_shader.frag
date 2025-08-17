#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 spriteColor;
uniform sampler2D image;
uniform bool is_brick;
uniform bool is_ball;
uniform bool has_texture;
uniform vec2 brick_size;
uniform bool hit;

// Optional: pass time for animations
uniform float time;

void main()
{
    vec2 uv = 2.0 * TexCoords - 1.0;
    if(has_texture)
        FragColor = texture(image, TexCoords) * vec4(spriteColor, 1.0);
    else if(!has_texture && is_ball)
        FragColor = vec4(spriteColor, step(length(uv), 1.0));
    else
        FragColor =  vec4(spriteColor, 1.0);
}
