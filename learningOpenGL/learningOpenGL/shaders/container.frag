#version 440

out vec4 FragColor;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
	// ambient
	vec3 ambient = lightColor * material.ambient; // amount of ambient light given by light

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // direction vector b/w lightsource and obj
	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = lightColor * (diff * material.diffuse);

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess); // power reps. the shinniness of the surface (amount it reflects)
	vec3 specular = lightColor * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}