#include <stdio.h>
#include <iostream>

#include "loguru.hpp"

import window;
import layer;
import world_layer;
import gui_layer;
import layer_stack;

int main(int argc, char** argv)
{
    loguru::g_preamble_date = false;
    loguru::g_preamble_thread = false;
    loguru::g_preamble_time = false;
    loguru::init(argc, argv);
  

    LayerStack layer_stack;
	Layer* world_layer = new WorldLayer("world");
	layer_stack.add_layer(world_layer);

	Layer* gui_layer = new GUILayer("gui");
	layer_stack.add_layer(gui_layer);


    int display_w = 0;
    int display_h = 0;

    if (!init_window())
    {
        return 1;
    }

	GLFWwindow* window = get_window();
    for (auto layer : layer_stack)
    {
		layer->init(window);
	}
 
    while (!window_should_close())
    {
        // events
        poll_events();
		get_window_size(display_w, display_h);

        // update 
        for (auto layer : layer_stack)
        {
            layer->update();
        }

        // render
		for (auto layer : layer_stack)
		{
            layer->begin();
            layer->render(display_w, display_h);
            layer->end();
		}

        // swap buffers
        present();
    }

	for (auto layer : layer_stack)
	{
		layer->cleanup();
	}
    
    cleanup_window();
    return 0;
}
