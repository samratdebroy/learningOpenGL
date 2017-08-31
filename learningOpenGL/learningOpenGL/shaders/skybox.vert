#version 440 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
// no Model uniform b/c skybox is located at world origin (0,0,0)

void main()
{
	TexCoords = aPos;
	vec4 pos = projection * view * vec4(aPos,1.0);
	gl_Position = pos.xyww; // set z=w so that at max depth of z=1.0f
}