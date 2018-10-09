#version 410

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;

void main()
{
	gl_Position = projection * view * vertexPosition;
	normal = vertexNormal;
}