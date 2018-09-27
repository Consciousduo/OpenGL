#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class ObjLoader
{
public:
	ObjLoader() {}
	~ObjLoader() {}

	bool loadFile(const char* path, std::vector<glm::vec3>* out_vertices, std::vector<glm::vec2>* out_uvs, std::vector<glm::vec3>* out_normals);
};