#define GLFW_INCLUDE_GL3 1
#include <GL/glfw.h>
#include "GL3/gl3w.h"

#include "vec.hpp"
#include "mat.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include <iostream>

using namespace hw::math;

void APIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	if ((type != GL_DEBUG_TYPE_PERFORMANCE_ARB && type != GL_DEBUG_TYPE_OTHER_ARB) || severity != GL_DEBUG_SEVERITY_LOW_ARB)
		std::cerr << message << std::endl;
	if ((type != GL_DEBUG_TYPE_PERFORMANCE_ARB && type != GL_DEBUG_TYPE_OTHER_ARB) || severity == GL_DEBUG_SEVERITY_HIGH_ARB)
		__debugbreak(); // Breakpoint
}

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

	if (glDebugMessageCallbackARB) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
		glDebugMessageCallbackARB(debug_callback, 0);
	}

	vec3 v = {1.0f, 0.0f, 0.0f};
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	mat4 m = {{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	}};

	using namespace hw::rsrc;

	ResourceManager resource_manager;
	TextureHandle h = resource_manager.loadTexture("placeholder");
	const hw::gl::Texture& gl_tex = h->getGLTexture();

	bool running = true;
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gl_tex.name);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2f(-0.75f, -0.75f);
			glTexCoord2f(0, 1);
			glVertex2f(-0.75f, 0.75f);
			glTexCoord2f(1, 1);
			glVertex2f(0.75f, 0.75f);
			glTexCoord2f(1, 0);
			glVertex2f(0.75f, -0.75f);
		glEnd();

		glfwSwapBuffers();
		running = running && glfwGetWindowParam(GLFW_OPENED);
	}

	glfwCloseWindow();
	glfwTerminate();
}