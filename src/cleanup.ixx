module;

#include <SDL3/SDL.h>

export module cleanup;

import sdl_data;

export void cleanup()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
