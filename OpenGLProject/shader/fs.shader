#version 410

in vec3 normal;
out vec4 color;

const vec3 ambient = vec3(0.2, 0.2, 0.2);
const vec3 lightVecNormalized1 = normalize(vec3(1.0, 1.0, -1.0));
vec3 lightColor = vec3(0.1, 0.7, 0.6);

void main()
{
	float diffuse1 = clamp(dot(lightVecNormalized1, normalize(normal)), 0.0, 1.0);
	color = vec4(ambient + diffuse1*lightColor, 1.0);
}