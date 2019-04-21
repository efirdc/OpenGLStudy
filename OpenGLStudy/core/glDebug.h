#ifndef GLDEBUG_H
#define GLDEBUG_H

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

void glDebug_init();

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam);

#endif