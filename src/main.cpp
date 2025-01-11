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
import special_actions;
import camera;


int main(int argc, char** argv)
{
	Layer* world_layer = new WorldLayer("world");
	get_layer_stack().add_layer(world_layer);

	Layer* gui_layer = new GUILayer("gui");
	get_layer_stack().add_layer(gui_layer);

    if (!init_window())
    {
        return 1;
    }


    for (auto layer : get_layer_stack())
    {
		layer->init();
	}

    get_persistent_data().load();
    get_updatable_manager().add_updatable(&get_camera());
    // ensure the special actions object is constructed
    get_special_actions();

    while (!window_should_close())
    {
        // events
        poll_events();

        update();
        render();
        present();
    }

	for (auto layer : get_layer_stack())
	{
		layer->cleanup();
	}
    
    cleanup_window();
    get_persistent_data().save();

	delete world_layer;
	delete gui_layer;

    return 0;
}
