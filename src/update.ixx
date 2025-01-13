module;

#include <SDL3/SDL.h>

export module update;

import sdl_data;
import layer_stack;
import updatable_manager;


export SDL_AppResult update()
{
	Uint8 r;

	/* fade between shades of red every 3 seconds, from 0 to 255. */
	r = (Uint8)((((float)(SDL_GetTicks() % 3000)) / 3000.0f) * 255.0f);
	SDL_SetRenderDrawColor(renderer, r, 0, 0, 255);

	/* you have to draw the whole window every frame. Clearing it makes sure the whole thing is sane. */
	SDL_RenderClear(renderer);  /* clear whole window to that fade color. */

	/* set the color to white */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	/* draw a square where the mouse cursor currently is. */
	SDL_RenderFillRect(renderer, &mouseposrect);

	/* put everything we drew to the screen. */
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;

	/*
	// update updatables
	get_updatable_manager().update_all();

	// update layers
	for (auto layer : get_layer_stack())
	{

		layer->update();

	}
	*/

};