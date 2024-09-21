module;

#include <string>
#include <functional>
#include "loguru.hpp"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

export module layer;

import data_listener;
import fbo;
import gui_data;

export class Layer
{
private:
	std::string name;
	FBO *fbo = nullptr;
	int width = 800;
	int height = 600;

public:
	Layer(std::string pname) : name(pname) {};
	~Layer() {};
	virtual void init(GLFWwindow *) {
		LOG_F(INFO, "Layer %s initialized", name.c_str());
	};
	void add_fbo()
	{
		fbo = new FBO(width, height);
	}
	void remote_fbo()
	{
		delete fbo;
		fbo = nullptr;
	}
	virtual void update() {};
	virtual void begin() {};
	virtual void render(int, int) {};
	virtual void end() {};
	virtual void cleanup() {
		LOG_F(INFO, "Layer %s cleaned up", name.c_str());
	};

	template <typename T>
	void add_listener(DataListener* listener, void (T::*func)()) {
		auto bound_func = std::bind(func, static_cast<T*>(this));
		listener->add_listener(bound_func);
	}

	void resize() {
		if (fbo != nullptr) {
			fbo->set_width(gui_data.viewport_width);
			fbo->set_height(height);
		}
	}

	std::string get_name() { return name; };
};