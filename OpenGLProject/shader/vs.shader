#version 410

layout(location = 1) in vec3 vertexNormal;
in vec3 position;
out vec3 theNormal;

void main()
{
   gl_Position = vec4(position, 1.0);
   theNormal = vertexNormal;
}