// This must be defined when statically linking to GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderUtil.h"


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

		// Points for triangle
		float points[6] = {

			// Left
			-0.8f, -0.5f,

			// Top
			0.0f, 0.9f,

			// Right
			0.5f, -0.7f
		};

		unsigned int buffer;

		// Create a buffer
		glGenBuffers(1, &buffer);

		// Bind the buffer to vertx attributes
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		// Init buffer
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), points, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		shaderUtil.Use();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw triangle
			glDrawArrays(GL_TRIANGLES, 0, 3);

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