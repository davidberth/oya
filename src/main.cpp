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
import update;
import gui_data;
import keyboard_data;
import mouse_data;
import viewport_data;
import persistent_data;
import updatable_manager;
import camera;
import window_data;

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


    if (!init_window())
    {
        return 1;
    }

	GLFWwindow* window = get_window();
    for (auto layer : layer_stack)
    {
		layer->init(window);
	}

    persistent_data.load();
    updatable_manager.add_updatable(&camera);

    /*
    if (persistent_data.gui_on)
    {
       world_layer->add_fbo();
	   viewport_data.active = true;
    }

    else
	{
    */
    background_layer->disable();
    //gui_layer->disable();
    viewport_data.active = false;
    viewport_data.height = window_data.height;
    viewport_data.width = window_data.width;
	//}

    while (!window_should_close())
    {
        // events
        poll_events();

        update();
        render();
        present();
    }

	for (auto layer : layer_stack)
	{
		layer->cleanup();
	}
    
    cleanup_window();
    persistent_data.save();

	delete background_layer;
	delete world_layer;
	delete gui_layer;

    return 0;
}
