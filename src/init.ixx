module;

#include <SDL3/SDL.h>

export module init;

import sdl_data;

export SDL_AppResult init()
{
	SDL_SetAppMetadata("SDL Hello World Example", "1.0", "com.example.sdl-hello-world");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Hello SDL", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	mouseposrect.x = mouseposrect.y = -1000;
	mouseposrect.w = mouseposrect.h = 50;

	return SDL_APP_CONTINUE;
}