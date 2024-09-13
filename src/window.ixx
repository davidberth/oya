module;

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <string>
#include "configure.h"
#include "loguru.hpp"

export module window;

import keyboard_data;
import mouse_data;

GLFWwindow* window;
int resolution_width;
int resolution_height;
GLFWcursor* cursor;

struct size_option
{
	int index;
	int key;
	float size_ratio_x;
	float size_ratio_y;
	int monitor;
};

size_option size_options[] =
{ {0, GLFW_KEY_F1, 0.2f, 0.3f, 0},
  {1, GLFW_KEY_F2, 0.4f, 0.5f, 0},
  {2, GLFW_KEY_F3, 0.6f, 0.7f, 0},
  {3, GLFW_KEY_F4, 0.8f, 0.85f, 0},
  {4, GLFW_KEY_F5, 1.0f, 1.0f, 0},
  {5, GLFW_KEY_F6, 1.0f, 1.0f, 1},
};

int current_size = 2;

void glfw_errorCallback(int error, const char* description)
{
	LOG_F(INFO, "GLFW error: %d description: %s", error, description);
}

void resize_window()
{

	float size_rat = size_options[current_size].size_ratio_x;

	if (size_rat > 0.9f)
	{
		int monitor_count;
		GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
		int index = size_options[current_size].monitor;
		if (index > monitor_count - 1) index = 0;

		const GLFWvidmode* mode = glfwGetVideoMode(monitors[index]);

		int mwidth = mode->width;
		int mheight = mode->height;

		glfwSetWindowMonitor(window, monitors[index], 0, 0, mwidth, mheight, GLFW_DONT_CARE);
		return;
	}
	else
	{
		int sizex = int(float(resolution_width) * size_options[current_size].size_ratio_x);
		int sizey = int(float(resolution_height) * size_options[current_size].size_ratio_y);
		glfwSetWindowMonitor(window, NULL, resolution_width / 2 - sizex / 2, resolution_height / 2 - sizey / 2, sizex, sizey, 0);

	}
}

void glfw_key_callback(GLFWwindow* lwindow, int key, int scancode, int action, int mods)
{
	for (auto& option : size_options)
	{
		if (key == option.key && action == GLFW_PRESS)
		{
			current_size = option.index;
			resize_window();
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(lwindow, GLFW_TRUE);

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_LEFT_CONTROL) keyboard_data.left_control_down = true;
		else if (key == GLFW_KEY_LEFT_SHIFT) keyboard_data.left_shift_down = true;
		keyboard_data.trigger();
	
	}
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_LEFT_CONTROL) keyboard_data.left_control_down = false;
		else if (key == GLFW_KEY_LEFT_SHIFT) keyboard_data.left_shift_down = false;
	
	}
	keyboard_data.log();

}

void glfw_scroll_callback(GLFWwindow* lwindow, double xoffset, double yoffset)
{
	mouse_scroll_data.yoffset = yoffset;
	mouse_scroll_data.trigger();
}

void glfw_mouse_button_callback(GLFWwindow* lwindow, int button, int action, int mods)
{
	mouse_button_data.button = button;
	mouse_button_data.trigger();
}


export void get_mouse_pos(double& xpos, double& ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
}

export int get_mouse_button_state()
{
	uint8_t state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	state += glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) << 1;
	state += glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) << 2;
	return state;
}

export bool init_window()
{
	glfwSetErrorCallback(glfw_errorCallback);
	if (!glfwInit())
		return false;

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	resolution_width = mode->width;
	resolution_height = mode->height;

	static std::string caption = APPNAME;
	caption[0] = std::toupper(caption[0]);
	caption += " Version: ";
	caption += APPVERSION;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	window = glfwCreateWindow(1280, 720, caption.c_str(), nullptr, nullptr);
	if (window == nullptr)
		return 1;
	glfwMakeContextCurrent(window);

	if (GLEW_OK != glewInit())
	{
		// GLEW failed!
		LOG_F(ERROR, "Failed to initialize GLEW\n");
		return 1;
	}

	resize_window();
	glfwSetKeyCallback(window, glfw_key_callback);
	glfwSetScrollCallback(window, glfw_scroll_callback);
	glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);

	cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(window, cursor);

	glfwSwapInterval(1);

	return true;
}

export GLFWwindow* get_window()
{
	return window;
}

export inline void get_window_size(int& width, int& height)
{

	glfwGetWindowSize(window, &width, &height);
}

export bool window_should_close()
{
	return glfwWindowShouldClose(window);
}

export void poll_events()
{
	glfwPollEvents();
}

export void present()
{
	glfwSwapBuffers(window);
}

export void cleanup_window()
{

	glfwDestroyWindow(window);
	glfwTerminate();
}
