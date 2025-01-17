module;

#include <SDL3/SDL.h>

export module keyboard;

import key_event;

export bool handle_keyboard_events(SDL_Event* event)
{
	if (event->key.key == SDLK_ESCAPE)
	{
		return true;
	}

	KeyEvent kevent(event->key.key, event->key.down);
	get_event_dispatcher().dispatch(kevent);

	return false;
}