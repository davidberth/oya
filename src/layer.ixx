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
	void add_fbo(int index)
	{
		fbo = new FBO(index);
	}
	void remove_fbo()
	{
		delete fbo;
		fbo = nullptr;
	}
	virtual void update() {};
	virtual void begin() {
		if (fbo)
		{
			fbo->bind();
		}
	};
	virtual void render() {};
	virtual void end() {
		if (fbo)
		{
			fbo->unbind();
		}
	};
	virtual void cleanup() {
		LOG_F(INFO, "Layer %s cleaned up", name.c_str());
		if (fbo)
		{
			delete fbo;
		}
	};

	std::string get_name() { return name; };
};