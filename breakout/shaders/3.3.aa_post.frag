#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 offsets[9];
uniform float blur_kernel[9];
uniform bool shake;

void main()
{
    FragColor = vec4(0.0f);
    vec3 samples[9];

    for (int i = 0; i < 9; ++i)
    {
        samples[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    if(shake)
    {
        for (int i = 0; i < 9; ++i)
        {
             FragColor += vec4(samples[i] * blur_kernel[i], 0.0f);
        }
        FragColor.a = 1.0;
    }
    else
        FragColor = texture(screenTexture, TexCoords);
}