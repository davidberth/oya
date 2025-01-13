module;

#include <SDL3/SDL.h>

export module keyboard;

export void;

export bool handle_keyboard_events(SDL_Event* event)
{
	if (event->key.scancode == SDLK_ESCAPE)
	{
		return true;
	}
	return false;
}