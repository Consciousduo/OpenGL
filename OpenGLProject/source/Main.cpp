// This must be defined when statically linking to GLEW
#define GLEW_STATIC
#define TINYOBJLOADER_IMPLEMENTATION
#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderUtil.h"
#include "tiny_obj_loader.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.16f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float cameraSpeed = 0.008f;

	if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* key call back */
	glfwSetKeyCallback(window, key_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	else
	{
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		// TODO: Create and compile shaders here (vertex and fragment shaders)
		// and finally draw something with modern OpenGL!
		ShaderUtil shaderUtil;
		shaderUtil.Load("shader/vs.shader", "shader/fs.shader");
		shaderUtil.Use();

		//Loading Object
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string error;
		const char* filename = "obj/rabbit/rabbit.obj";
		//const char* filename = "obj/teapot/teapot.obj";
		//const char* filename = "obj/lambo/lambo.obj";
		tinyobj::LoadObj(&attrib, &shapes, &materials, &error, filename);

		std::vector <glm::vec3> vertices;
		std::vector <glm::vec3> normals;
		std::vector <glm::vec2> uvs;

		std::vector <glm::vec3> temp_vertices;
		std::vector <glm::vec3> temp_normals;
		std::vector <glm::vec2> temp_uvs;

		for (int i = 0; i < attrib.vertices.size() / 3; i++) {
			temp_vertices.push_back(glm::vec3(attrib.vertices[i * 3 + 0], attrib.vertices[i * 3 + 1], attrib.vertices[i * 3 + 2]));
		}
		for (int i = 0; i < attrib.normals.size() / 3; i++) {
			temp_normals.push_back(glm::vec3(attrib.normals[i * 3 + 0], attrib.normals[i * 3 + 1], attrib.normals[i * 3 + 2]));
		}
		for (int i = 0; i < attrib.texcoords.size() / 2; i++) {
			temp_uvs.push_back(glm::vec2(attrib.texcoords[i * 2 + 0], attrib.texcoords[i * 2 + 1]));
		}

		std::vector<tinyobj::index_t> indices = shapes[0].mesh.indices;
		for (int i = 0; i < indices.size(); i++) {
			vertices.push_back(temp_vertices[indices[i].vertex_index]);
			normals.push_back(temp_normals[indices[i].normal_index]);
			uvs.push_back(temp_uvs[indices[i].texcoord_index]);
		}



		GLuint vertexArrayObj;
		glGenVertexArrays(1, &vertexArrayObj);
		glBindVertexArray(vertexArrayObj);

		GLuint VertexBuffer, NormalBuffer, UVBuffer;
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			//camera
			glm::mat4 model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 view = glm::lookAt(cameraPos, cameraLookAt, cameraUp);
			glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.001f, 1000.0f);
			
			// Get the uniform locations
			GLint modelLoc = glGetUniformLocation(shaderUtil.getProgramId(), "model");
			GLint viewLoc = glGetUniformLocation(shaderUtil.getProgramId(), "view");
			GLint projLoc = glGetUniformLocation(shaderUtil.getProgramId(), "projection");

			// Pass the matrices to the shader
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Draw triangle
			glBindVertexArray(vertexArrayObj);
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glBindVertexArray(0);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
			/* Poll for and process events */
			glfwPollEvents();
		}
		shaderUtil.Delete();
	}

	glfwTerminate();
	return 0;
}
