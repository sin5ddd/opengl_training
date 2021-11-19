//
// Created by kitayama on 2021/11/19.
//

#ifndef UNTITLED2_WINDOW_H
#define UNTITLED2_WINDOW_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
	GLFWwindow* const window;
	GLfloat size[2];
	GLfloat scale;
	GLfloat location[2];
	int keyStatus;

public:
	Window(int width = 640, int height = 480, const char* title = "Hello!")
			: window(glfwCreateWindow(width, height, title, nullptr, nullptr)),
			  scale(100.0f),
			  location{0.0f, 0.0f},
			  keyStatus(GLFW_RELEASE) {
		if(window == nullptr) {
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		glfwMakeContextCurrent(window);
		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK) {
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}

		glfwSwapInterval(1); //vsync on
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, wheel);
		glfwSetKeyCallback(window, keyboard);
		glfwSetWindowSizeCallback(window, resize);
		resize(window, width, height);
	}

	virtual ~Window() {
		glfwDestroyWindow(window);
	}

	// 実質的メインループ
	explicit operator bool() {
		if(keyStatus == GLFW_RELEASE) {
			glfwWaitEvents();
		} else {
			glfwPollEvents();
		}

		auto mv = 2.0f;
		if(glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE) {
			location[0] -= mv / size[0];
		} else if(glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE) {
			location[0] += mv / size[0];
		} else if(glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE) {
			location[1] += mv / size[1];
		} else if(glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE) {
			location[1] -= mv / size[1];
		}

		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE) {

			double x, y;
			glfwGetCursorPos(window, &x, &y);
			location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
			location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1]; // invert
		}
		return !glfwWindowShouldClose(window);
	}

	void swapBuffers() const {
		glfwSwapBuffers(window);
	}

	static void resize(GLFWwindow* const window, int width, int height) {
		int fb_width, fb_height;
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		glViewport(0, 0, fb_width, fb_height);

		auto const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != nullptr) {
			instance->size[0] = static_cast<GLfloat>(width);
			instance->size[1] = static_cast<GLfloat>(height);
		}
	}

	const GLfloat* getSize() const { return size; }

	GLfloat getScale() const { return scale; }

	const GLfloat* getLocation() const {
//		std::cerr << "mouse (x,y)=(" << location[0] << "," << location[1] << ")" << std::endl;
		return location;
	}

	static void wheel(GLFWwindow* window, double x, double y) {
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != nullptr) {
			instance->scale += static_cast<GLfloat>(y * 2);
		}
	}

	static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != nullptr) {
			instance->keyStatus = action;
		}
	}

};

#endif //UNTITLED2_WINDOW_H
