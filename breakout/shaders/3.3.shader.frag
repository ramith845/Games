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

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float noise(vec2 uv) {
    vec2 i = floor(uv);
    vec2 f = fract(uv);
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

float sdRoundedBox( in vec2 p, in vec2 b, in vec4 r )
{
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

void main()
{
    if (!is_brick) {
        FragColor = vec4(spriteColor, 1.0) * texture(image, TexCoords);
        return;
    }

    // Fancy but grounded gradient for bricks
    vec3 darkBase = vec3(0.08, 0.02, 0.12);   // near black-purple
    vec3 neonAccent = vec3(0.0, 0.6, 0.7);    // deep cyan
    
    // Diagonal gradient factor
    float g = smoothstep(0.2, 0.8, (TexCoords.x + TexCoords.y) * 0.5);
    
    // Blend gradient with spriteColor (stronger spriteColor influence)
    vec3 blended = mix(darkBase, neonAccent, g);
    vec3 color = mix(blended, spriteColor, 0.5);
    
    // Optional: subtle noise to break flatness (optional)
    float grain = fract(sin(dot(TexCoords * 100.0, vec2(12.9898,78.233))) * 43758.5453);
    color += grain * 0.03;
    
    // Clamp to avoid blowout
    color = clamp(color, 0.0, 1.0);
    color += pow(color, vec3(1.5));


    vec2 uv = 2.0 * TexCoords - 1.0;
    uv.x *= brick_size.x/ brick_size.y;
    float outer = sdRoundedBox(uv, vec2(1.2, 0.75), vec4(0.1));
    vec3 brightnes_mask = vec3(smoothstep(0.0, 0.1,outer));

    color += brightnes_mask*1.1;
    // Final color
    vec4 debug = vec4(1.0);
    FragColor =  debug * vec4(spriteColor, 1.0) * texture(image, TexCoords);
    //if(hit)
    //  FragColor = vec4(1.0, 0.2, 0.1, 1.0);

}
