module;

#include <string>
#include <functional>
#include "loguru.hpp"
#include <GLFW/glfw3.h>

export module layer;

import data_listener;

export class Layer
{
private:
	std::string name;
public:
	Layer(std::string pname) : name(pname) {};
	~Layer() {};
	virtual void init(GLFWwindow *) {
		LOG_F(INFO, "Layer %s initialized", name.c_str());
	};
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

	std::string get_name() { return name; };
};