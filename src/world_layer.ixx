module;

#include <string>
#include "loguru.hpp"

#include <GLFW/glfw3.h>

export module world_layer;

import layer;

export class WorldLayer : public Layer
{
public:
	WorldLayer(std::string pname) : Layer(pname) {};
	~WorldLayer() {};
	virtual void init(GLFWwindow* window) override
	{
		__super::init(window);

	}
	virtual void update() override
	{

	}
	virtual void begin() override
	{
	}
	virtual void render(int display_w, int display_h) override
	{
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.6f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	}
	virtual void end() override
	{
	}
	virtual void cleanup() override
	{
	}
};