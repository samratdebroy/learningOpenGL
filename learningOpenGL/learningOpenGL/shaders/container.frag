#version 440

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); // amount of ambient light given by light

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); // direction vector b/w lightsource and obj
	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff *  vec3(texture(material.diffuse, TexCoords)));

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess); // power reps. the shinniness of the surface (amount it reflects)
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}