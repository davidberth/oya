module;

#include <string>
#include "loguru.hpp"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

export module layer;

import listener;
import fbo;
import gui_data;

export class Layer : public Listener
{
private:
	std::string name;
	FBO *fbo = nullptr;
	
public:
	Layer(std::string pname) : Listener(), name(pname) {}
	~Layer() {};
	virtual void init(GLFWwindow *) {
		LOG_F(INFO, "Layer %s initialized", name.c_str());
	};
	void add_fbo()
	{
		fbo = new FBO(800, 800);
	}
	void remove_fbo()
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

	std::string get_name() { return name; };
};