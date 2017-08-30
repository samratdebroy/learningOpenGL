#version 440 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	FragPos = vec3(model * vec4(aPos,1.0)); // Retrieve the world position of the fragment
	Normal = mat3(transpose(inverse(model))) * aNormal; // this ensures that uneven scaling won't distort the normal vector, but is costly to do on shader.

	gl_Position = projection * view * vec4(FragPos,1.0);
	TexCoords = aTexCoords;
}