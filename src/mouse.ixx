module;

#include <SDL3/SDL.h>

export module mouse;

import sdl_data;
import mouse_event;

export void handle_mouse_movement(SDL_Event* event)
{
	MouseMoveEvent mevent(event->motion.x, event->motion.y);
	get_event_dispatcher()->dispatch(mevent);

}

export void handle_mouse_scroll(SDL_Event* event)
{
	MouseScrollEvent mevent(event->wheel.x, event->wheel.y);
	get_event_dispatcher()->dispatch(mevent);
}