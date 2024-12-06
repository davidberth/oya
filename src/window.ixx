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






void glfw_errorCallback(int error, const char* description)
{
	LOG_F(INFO, "GLFW error: %d description: %s", error, description);
}

export void set_window_size(float size_ratio_x, float size_ratio_y, int monitor)
{

	if (size_ratio_x > 0.99f)
	{
		// full screen
		int monitor_count;
		GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
		int index = monitor;
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
		int sizex = int(float(resolution_width) * size_ratio_x);
		int sizey = int(float(resolution_height) * size_ratio_y);
		glfwSetWindowMonitor(window, NULL, resolution_width / 2 - sizex / 2, resolution_height / 2 - sizey / 2, sizex, sizey, 0);
		glViewport(0, 0, sizex, sizey);
		window_data.width = sizex;
		window_data.height = sizey;
	}
}

void glfw_key_callback(GLFWwindow* lwindow, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(lwindow, GLFW_TRUE);


	if (action == GLFW_PRESS)
	{
		KeyEvent event(key, GLFW_PRESS);
		event_dispatcher.dispatch(event);
	}
	else if (action == GLFW_RELEASE)
	{
		KeyEvent event(key, GLFW_RELEASE);
		event_dispatcher.dispatch(event);
	}

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
	
	set_window_size(0.5f, 0.5f, 0);
	glfwSetKeyCallback(window, glfw_key_callback);
	glfwSetScrollCallback(window, glfw_scroll_callback);
	glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);
	glfwSetWindowRefreshCallback(window, glfw_refresh_callback);
	glfwSetCursorPosCallback(window, glfw_cursor_position_callback);

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
