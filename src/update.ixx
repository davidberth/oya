module;

#include <SDL3/SDL.h>

export module update;

import sdl_data;
import layer_stack;
import updatable_manager;


export SDL_AppResult update()
{

	// update updatables
	get_updatable_manager().update_all();

	// update layers
	for (auto layer : get_layer_stack())
	{

		layer->update();

	}

	return SDL_APP_CONTINUE;
};