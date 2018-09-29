// This must be defined when statically linking to GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderUtil.h"
#include "ObjLoader.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>



int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000, 1000, "Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

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

		//Load Obj File
		std::vector <glm::vec3> vertices;
		std::vector <glm::vec2> uvs;
		std::vector <glm::vec3> normals;
		ObjLoader objLoader;
		objLoader.loadFile("obj/teapot/teapot.obj", &vertices, &uvs, &normals);

		
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
		
		
		
		//camera
		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 0.0f, -100.0f),
			glm::vec3(0.0f, 0.03f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 projection;
		projection = glm::perspective(45.0f, 1.0f, 0.01f, 1000.0f);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(shaderUtil.getProgramId(), "model");
		GLint viewLoc = glGetUniformLocation(shaderUtil.getProgramId(), "view");
		GLint projLoc = glGetUniformLocation(shaderUtil.getProgramId(), "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

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
