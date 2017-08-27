#version 440 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1; // Passes texture obj to shader
uniform sampler2D texture2; // Passes texture obj to shader

void main()
{
    FragColor = mix(texture(texture1, TexCoord),texture(texture2, TexCoord),0.2); // Sample the colors from the texture 80% from 1 and 20% from 2
}