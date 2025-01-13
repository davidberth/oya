module;

#include <SDL3/SDL.h>

export module mouse;

import sdl_data;

export void handle_mouse_movement(SDL_Event* event)
{
	mouseposrect.x = event->motion.x - (mouseposrect.w / 2);
	mouseposrect.y = event->motion.y - (mouseposrect.h / 2);
}