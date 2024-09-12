module;

#include <string>
#include <functional>
#include "loguru.hpp"
#include <GLFW/glfw3.h>

export module world_layer;

import layer;
import gui_data;
import data_listener;
import mouse_data;

export class WorldLayer : public Layer
{
public:
	WorldLayer(std::string pname) : Layer(pname) {};
	~WorldLayer() {};
	virtual void init(GLFWwindow* window) override
	{
		__super::init(window);
		add_listener(&mouse_button_data, &WorldLayer::on_mouse_button);
		
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
		glClearColor(gui_data.clear_color[0], 
					 gui_data.clear_color[1],
					 gui_data.clear_color[2],
					 gui_data.clear_color[3]);
		glClear(GL_COLOR_BUFFER_BIT);

	}
	virtual void end() override
	{
	}
	virtual void cleanup() override
	{
		__super::cleanup();
	}

	// listeners
	void on_mouse_button(DataListener* listener)
	{
		auto mouse_listener = dynamic_cast<MouseButtonData*>(listener);
		LOG_F(INFO, "On Mouse Called!");
		mouse_listener->log();
	}

};