#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <memory>
#include "Window.h"
#include "Shape.h"

bool readShaderSource(const char* name, std::vector<GLchar>& buffer) {
	if(name == nullptr) { return false; }
	std::ifstream file(name, std::ios::binary);
	if(file.fail()) {
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}

	// get file length
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	buffer.resize(length + 1);
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if(file.fail()) {
		std::cerr << "Error: Could not read source file: " << name << std::endl;
		file.close();
		return false;
	}

	file.close();
	return true;
}

GLboolean printShaderInfoLog(GLuint shader, const char* str) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		std::cerr << "Compile Error in " << str << std::endl;
	}

	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
	if(bufSize > 1) {
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

GLboolean printProgramInfoLog(GLuint program) {
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		std::cerr << "Link Error." << std::endl;
	}
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if(bufSize > 1) {
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

GLuint createProgram(const char* vsrc, const char* fsrc) {
	const auto program = glCreateProgram();
	if(vsrc != nullptr) {
		const auto obj = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(obj, 1, &vsrc, nullptr);
		glCompileShader(obj);
		if(printShaderInfoLog(obj, "vertex shader")) {
			glAttachShader(program, obj);
		}
		glDeleteShader(obj);
	}
	if(fsrc != nullptr) {
		const auto obj = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(obj, 1, &fsrc, nullptr);
		glCompileShader(obj);
		if(printShaderInfoLog(obj, "fragment shader")) {
			glAttachShader(program, obj);
		}
		glDeleteShader(obj);
	}

	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	if(printProgramInfoLog(program)) {
		return program;
	}
	glDeleteProgram(program);
	return 0;
}

GLuint loadProgram(const char* vert, const char* frag) {
	std::vector<GLchar> vsrc, fsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	const bool fstat(readShaderSource(frag, fsrc));
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

constexpr Object::Vertex rectangleVertex[] = {
		{-0.5f, -0.5f},
		{0.5f,  -0.5f},
		{0.5f,  0.5f},
		{-0.5f, 0.5f},
};


int main() {
	if(glfwInit() == GL_FALSE) {
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	atexit(glfwTerminate);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window window;

	glClearColor(1.0f, 0.9f, 1.0f, 0.5f);
//	glViewport(100, 50, 300, 300);

	const GLuint program = loadProgram("shaders/point.vert", "shaders/point.frag");
//	const auto aspectLoc(glGetUniformLocation(program, "aspect"));
	const GLint sizeLoc = glGetUniformLocation(program, "size");
	const GLint scaleLoc = glGetUniformLocation(program, "scale");
	const GLint locationLoc = glGetUniformLocation(program, "location");

	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));

	while (window) {
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);

		// set uniform
//		glUniform1f(aspectLoc, window.getAspect());
		glUniform2fv(sizeLoc, 1, window.getSize());
		glUniform1f(scaleLoc, window.getScale());
		glUniform2fv(locationLoc, 1, window.getLocation());

		// render and draw
		shape->draw();

		window.swapBuffers();
	}

	return 0;
}
