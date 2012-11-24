#define GLFW_INCLUDE_GL3 1
#include <GL/glfw.h>
#include "GL3/gl3w.h"

#include "vec.hpp"
#include "mat.hpp"

using namespace hw::math;

int main() {
	if (!glfwInit()) {
		return 1;
	}

	if (!glfwOpenWindow(800, 600, 8, 8, 8, 0, 24, 8, GLFW_WINDOW)) {
		return 1;
	}

	if (gl3wInit()) {
		return 1;
	}

	if (!gl3wIsSupported(3, 3)) {
		return 1;
	}

	vec3 v = {1.0f, 0.0f, 0.0f};
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	mat4 m = {{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	}};

	bool running = true;
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers();
		running = running && glfwGetWindowParam(GLFW_OPENED);
	}

	glfwCloseWindow();
	glfwTerminate();
}