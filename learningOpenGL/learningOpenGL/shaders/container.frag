#version 440

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor; // amount of ambient light given by light

	vec3 result = ambient * objectColor;
	FragColor = vec4(result, 1.0);
}