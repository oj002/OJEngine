#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "Macros.h"

class Window
{
public:

	Window(int width, int height, const char* title);
	~Window();

	inline bool closed() const { return glfwWindowShouldClose(m_pWindow); }
	inline void pollEvent() { glfwPollEvents(); }

	inline bool isKeyPressed(int keyCode) const { return m_keys[keyCode]; }
	inline bool isMouseButtonPressed(int keyCode) const { return m_mouseButtons[keyCode]; }
	inline glm::vec2 getCursorPos() const { return m_mousePos; }

	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }

	inline GLFWwindow *getWindow() const { return m_pWindow; }

private:
	friend static void resize_callback(GLFWwindow *window, int width, int height);
	friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

private:
	GLFWwindow* m_pWindow;
	int m_width, m_height;
	bool m_keys[1024];
	bool m_mouseButtons[32];
	glm::vec2 m_mousePos;
};