#version 440 core

out vec4 FragColor;

in vec3 TexCoords; // Direction vector representing a 3D texture coordinate
uniform samplerCube skybox; // cubemap texture sampler

void main()
{
	FragColor =texture(skybox, TexCoords);
}