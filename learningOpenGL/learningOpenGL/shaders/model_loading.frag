#version 440

out vec4 FragColor;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPos;	
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight light, vec3 norm,vec3 fragPos, vec3 viewDir);  

void main()
{
	// setup fields
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Phase I : Directional Light
	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	// Phase II : Point Lights
	for (int i = 0 ; i < NR_POINT_LIGHTS ; i ++)
		result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
	// Phase III : Spotlight Light
    result += CalcSpotLight(spotLight, normal, FragPos, viewDir);    

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){

	vec3 lightDir = normalize(-light.direction);
	// Diffuse shading	
	float diff = max(dot(normal,lightDir),0.0); 
	// Specular Shading
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);
	// combine regions
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,TexCoords));
	return (ambient + diffuse + specular);
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){

	vec3 lightDir = normalize(light.position - fragPos);
	// Diffuse shading	
	float diff = max(dot(normal,lightDir),0.0); 
	// Specular Shading
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);

	// Attenuation
	float distance	= length(light.position - fragPos);
	float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic*(distance*distance));

	// combine regions
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);

};

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos, vec3 viewDir){


	vec3 lightDir = normalize(light.position - fragPos);
	// Diffuse shading	
	float diff = max(dot(normal,lightDir),0.0); 
	// Specular Shading
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);

	// Attenuation
	float distance	= length(light.position - fragPos);
	float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic*(distance*distance));

	// Intensity of spot light
	float tetha = dot(lightDir,normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((tetha - light.outerCutOff)/epsilon, 0.0,1.0);

	// combine regions
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,TexCoords));
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);
};