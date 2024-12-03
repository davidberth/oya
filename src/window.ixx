module;

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <string>
#include "configure.h"
#include "loguru.hpp"

export module window;

import keyboard_data;
import viewport_data;
import window_data;
import mouse_data;
import render;
import update;
import persistent_data;
import event;
import key_event;


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
	persistent_data.window_size = current_size;
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
		glViewport(0, 0, mwidth, mheight);
		window_data.width = mwidth;
		window_data.height = mheight;

	}
	else
	{
		int sizex = int(float(resolution_width) * size_options[current_size].size_ratio_x);
		int sizey = int(float(resolution_height) * size_options[current_size].size_ratio_y);
		glfwSetWindowMonitor(window, NULL, resolution_width / 2 - sizex / 2, resolution_height / 2 - sizey / 2, sizex, sizey, 0);
		glViewport(0, 0, sizex, sizey);
		window_data.width = sizex;
		window_data.height = sizey;
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
		KeyEvent event(key, GLFW_PRESS);
		event_dispatcher.dispatch(event);

	}
	/*
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_LEFT_CONTROL) keyboard_data.left_control_down = false;
		
		if (key == GLFW_KEY_F10) { function_keyboard_data.F10_down = false; }
		if (key == GLFW_KEY_F11) { function_keyboard_data.F11_down = false; }

		if (key == GLFW_KEY_A) keyboard_data.left_down = false;
		if (key == GLFW_KEY_D) keyboard_data.right_down = false;
		if (key == GLFW_KEY_W) keyboard_data.up_down = false;
		if (key == GLFW_KEY_S) keyboard_data.down_down = false;
		if (key == GLFW_KEY_Q) keyboard_data.rotate_left_down = false;
		if (key == GLFW_KEY_E) keyboard_data.rotate_right_down = false;

		if (key == GLFW_KEY_R) keyboard_data.zoom_out_down = false;
		if (key == GLFW_KEY_F) keyboard_data.zoom_in_down = false;

	
	}*/
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

void glfw_window_size_callback(GLFWwindow* lwindow, int width, int height)
{
	glViewport(0, 0, width, height);
	window_data.width = width;
	window_data.height = height;
	if (!viewport_data.active)
	{
		viewport_data.set_size(window_data.width, window_data.height);
	}

}

export void present()
{
	glfwSwapBuffers(window);
}

void glfw_refresh_callback(GLFWwindow* lwindow)
{
	update();
	render();
	present();
}

void glfw_cursor_position_callback(GLFWwindow* lwindow, double xpos, double ypos)
{
	mouse_pointer_data.xpos = xpos;
	mouse_pointer_data.ypos = ypos;
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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	current_size = persistent_data.window_size;
	resize_window();
	glfwSetKeyCallback(window, glfw_key_callback);
	glfwSetScrollCallback(window, glfw_scroll_callback);
	glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);
	glfwSetWindowRefreshCallback(window, glfw_refresh_callback);
	glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
	

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

export void cleanup_window()
{

	glfwDestroyWindow(window);
	glfwTerminate();
}
