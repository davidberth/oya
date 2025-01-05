#include <stdio.h>
#include <iostream>



import window;
import layer;
import world_layer;
import gui_layer;
import layer_stack;
import render;
import update;
import persistent_data;
import updatable_manager;
import camera;


int main(int argc, char** argv)
{
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

	delete world_layer;
	delete gui_layer;

    return 0;
}
