#define SDL_MAIN_USE_CALLBACKS
#include "imgui_impl_sdl3.h"
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


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	return init();
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
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


/*
import window;
import layer;
import world_layer;
import gui_layer;
import layer_stack;
import render;
import update;
import persistent_data;
import updatable_manager;
import special_actions;
import camera;
*/

/*
int main(int argc, char** argv)
{
	Layer* world_layer = new WorldLayer("world");
	get_layer_stack().add_layer(world_layer);

	Layer* gui_layer = new GUILayer("gui");
	get_layer_stack().add_layer(gui_layer);

	if (!init_window())
	{
		return 1;
	}


	for (auto layer : get_layer_stack())
	{
		layer->init();
	}

	get_persistent_data().load();
	get_updatable_manager().add_updatable(&get_camera());
	// ensure the special actions object is constructed
	get_special_actions();

	while (!window_should_close())
	{
		// events
		poll_events();

		update();
		render();
		present();
	}

	for (auto layer : get_layer_stack())
	{
		layer->cleanup();
	}

	cleanup_window();
	get_persistent_data().save();

	delete world_layer;
	delete gui_layer;

	return 0;
}
*/