// This must be defined when statically linking to GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderUtil.h"
#include "ObjLoader.h"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

		//Load Obj File
		std::vector <glm::vec3> vertices;
		std::vector <glm::vec2> uvs;
		std::vector <glm::vec3> normals;
		ObjLoader objLoader;
		objLoader.loadFile("obj/rabbit/rabbit.obj", &vertices, &uvs, &normals);

		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);


		shaderUtil.Use();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw triangle
			//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glBindVertexArray(VAO);
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