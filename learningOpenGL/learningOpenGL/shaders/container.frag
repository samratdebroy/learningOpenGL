#version 440

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Attenuation constants
	float constant;
	float linear;
	float quadratic;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos); // direction vector b/w lightsource and obj

	float theta = dot(lightDir, normalize(-light.direction)); // Angle between light direction and direction from light source to frag
	float epsilon  = light.cutOff - light.outerCutOff; // difference b/w the cosines of inner and outside light cutoffs
	float intensity = clamp( (theta - light.outerCutOff)/epsilon, 0.0, 1.0); // Set the intensity of the flashlight light (soft fade near outer edge)

if(theta > light.outerCutOff)
{
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); // amount of ambient light given by light

	// Diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm,lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff *  vec3(texture(material.diffuse, TexCoords)));

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess); // power reps. the shinniness of the surface (amount it reflects)
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic*(distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
else
{
	FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)),1.0); // If not under spotlight, still has ambient light
}


}