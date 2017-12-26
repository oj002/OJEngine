#include "Window.h"

Window::Window(int width, int height, const char * title)
	: m_pWindow(nullptr)
	, m_width(width)
	, m_height(height)
	, m_keys()
	, m_mouseButtons()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_pWindow == nullptr)
	{
		DEBUG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		exit(0);
	}
	glfwMakeContextCurrent(m_pWindow);
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, resize_callback);
	glfwSetKeyCallback(m_pWindow, key_callback);
	glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
	glfwSetCursorPosCallback(m_pWindow, cursor_position_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_ERROR("Failed to initialize GLAD");
		exit(0);
	}
}

Window::~Window()
{
	glfwTerminate();
}

void Window::pollEvent()
{
	for (int i = 0; i < 1024; i++)
	{
		if (i < 32)
		{
			m_mouseButtons[i].pressed = false;
			m_mouseButtons[i].released = false;
		}
		m_keys[i].pressed = false;
		m_keys[i].released = false;
	}
	glfwPollEvents();
}


void resize_callback(GLFWwindow * window, int width, int height)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	glfwGetFramebufferSize(window, &win->m_width, &win->m_height);
	GLCall(glViewport(0, 0, win->m_width, win->m_height));
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (action != GLFW_RELEASE)
	{
		win->m_keys[key].pressed = !win->m_keys[key].held;
		win->m_keys[key].held = true;
	}
	else
	{
		win->m_keys[key].released = true;
		win->m_keys[key].held = false;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (action != GLFW_RELEASE)
	{
		win->m_mouseButtons[button].pressed = !win->m_mouseButtons[button].held;
		win->m_mouseButtons[button].held = true;
	}
	else
	{
		win->m_mouseButtons[button].released = true;
		win->m_mouseButtons[button].held = false;
	}
}

void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	win->m_mousePos.x = static_cast<float>(xpos);
	win->m_mousePos.y = static_cast<float>(ypos);
}