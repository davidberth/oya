#include <stdio.h>
#include <iostream>

#include "loguru.hpp"

import window;
import layer;
import world_layer;
import gui_layer;
import background_layer;
import layer_stack;
import data_trigger_stack;
import render;

import gui_data;
import keyboard_data;
import mouse_data;
import viewport_data;

int main(int argc, char** argv)
{

    loguru::g_preamble_date = false;
    loguru::g_preamble_thread = false;
    loguru::g_preamble_time = false;
    loguru::init(argc, argv);

 
	Layer* background_layer = new BackgroundLayer("background");
	layer_stack.add_layer(background_layer);

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

    background_layer->disable();
    gui_layer->disable();


    while (!window_should_close())
    {
        // events
        poll_events();
		get_window_size(display_w, display_h);

        // update data triggers
		for (auto data_trigger : data_trigger_stack)
		{
			data_trigger->update();
		}


        // update layers
        for (auto layer : layer_stack)
        {
            if (layer->is_enabled()) layer->update();
        }

        render();
        present();
    }

	for (auto layer : layer_stack)
	{
		layer->cleanup();
	}
    
    cleanup_window();
    return 0;
}
