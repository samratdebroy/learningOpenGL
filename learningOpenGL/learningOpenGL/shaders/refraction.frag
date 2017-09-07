#version 440 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	float refractionRatio = 1.00 / 1.52; // 1.52 is the refraction index of Glass
	vec3 I = normalize(Position - cameraPos); // Incident ray from viewer to obj
	vec3 R = refract(I, normalize(Normal),refractionRatio); // Reflection of incident ray
	FragColor = vec4(texture(skybox, R).rgb, 1.0); // color equal to point of skybox we're pointing to
}