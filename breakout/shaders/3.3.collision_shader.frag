#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 spriteColor;
uniform sampler2D image;
uniform bool is_brick;
uniform vec2 brick_size;
uniform bool hit;

// Optional: pass time for animations
uniform float time;


float sdRoundedBox( in vec2 p, in vec2 b, in vec4 r )
{
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

void main()
{
    vec3 color = spriteColor;
    // Clamp to avoid blowout
    color = clamp(color, 0.0, 1.0);
    color += pow(color, vec3(1.5));


    vec2 uv = 2.0 * TexCoords - 1.0;
    uv.x *= brick_size.x/ brick_size.y;
    float outer = sdRoundedBox(uv, vec2(1.2, 0.75), vec4(0.1));
    vec3 brightnes_mask = vec3(smoothstep(0.0, 0.1,outer));

    color += brightnes_mask*1.1;
    FragColor =  vec4(color, 1.0);
}
