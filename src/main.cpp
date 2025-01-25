#define SDL_MAIN_USE_CALLBACKS
#include "imgui_impl_sdl3.h"
#include <chrono>  
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

import init;
import render;
import update;
import keyboard;
import mouse;
import cleanup;
import event;
import window_event;
import frame_rate;


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	return init();
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	log_frame_rate();

	update();
	render();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

	ImGui_ImplSDL3_ProcessEvent(event);
	switch (event->type) {
	case SDL_EVENT_KEY_DOWN:
		if (handle_keyboard_events(event)) return SDL_APP_SUCCESS;
		break;
	case SDL_EVENT_MOUSE_MOTION:
		handle_mouse_movement(event);
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		handle_mouse_scroll(event);
		break;
	case SDL_EVENT_WINDOW_RESIZED:
		int width = event->window.data1;
		int height = event->window.data2;
		get_event_dispatcher().dispatch(WindowEvent(width, height));
		break;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	cleanup();
}
