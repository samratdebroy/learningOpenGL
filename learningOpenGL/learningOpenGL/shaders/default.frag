#version 440 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture; // Passes texture obj to shader

void main()
{
    FragColor = texture(ourTexture, TexCoord); // Sample the colors from the texture
}