module;

#include <SDL3/SDL.h>

export module cleanup;

import sdl_data;

export void cleanup()
{
	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}
