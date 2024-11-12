module;

#include <string>
#include <glfw/glfw3.h>

export module background_layer;

import layer;
import gui_data;
import keyboard_data;

export class BackgroundLayer : public Layer
{
    
public:
    BackgroundLayer(std::string pname) : Layer(pname) {};
    ~BackgroundLayer() {};
    virtual void init(GLFWwindow* window) override
    {
        Layer::init(window);
        add_listener(&function_keyboard_data, &BackgroundLayer::on_function_keyboard, true);
    }
    virtual void update() override
    {

    }
    virtual void begin()
    {
        Layer::begin();
    }
    virtual void render() override
    {
        glClearColor(gui_data.clear_color[0],
            gui_data.clear_color[1],
            gui_data.clear_color[2],
            gui_data.clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

   

    }
    virtual void end() override
    {
        Layer::end();
    }
    virtual void cleanup() override
    {
        Layer::cleanup();
    }

    // listeners

    // persistent
	void on_function_keyboard()
	{
		if (function_keyboard_data.F10_down)
		{
			disable();

		}
		else if (function_keyboard_data.F11_down)
		{
			enable();
		}
	}


};

