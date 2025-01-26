module;

#include <SDL3/SDL.h>

export module cleanup;

import sdl_data;
import layer_stack;
import event;
import camera;
import input_manager;
import layer_stack;
import scene_serializer;
import special_actions;
import updatable_manager;
import viewport;

export void cleanup()
{
	SDL_WaitForGPUIdle(sdl_device);

	delete_event_dispatcher();
	delete_camera();
	delete_input_manager();
	delete_scene_serializer();
	delete_special_actions();
	delete_updatable_manager();
	delete_viewport();

	for (auto layer : *get_layer_stack())
	{
		layer->cleanup();
	}

	delete_layer_stack();

	SDL_ReleaseWindowFromGPUDevice(sdl_device, sdl_window);
	SDL_DestroyGPUDevice(sdl_device);
	//SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}
