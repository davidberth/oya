module;

#include <SDL3/SDL.h>

export module init;

import sdl_data;
import layer_stack;
import world_layer;
import gui_layer;


export SDL_AppResult init()
{
	SDL_SetAppMetadata("Oya", "1.0", "oya");

	SDL_Log("Initializing Video");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "vulkan");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	sdl_device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, true, "vulkan");
	SDL_Log("Creating GPU Device");
	if (!sdl_device) {
		SDL_Log("SDL_CreateGPUDevice() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_Log("Creating window");
	sdl_window = SDL_CreateWindow("Oya 0.1.1", 1000, 800, SDL_WINDOW_RESIZABLE);
	if (!sdl_window) {
		SDL_Log("SDL_CreateWindow() failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_Log(SDL_GetGPUDriver(0));
	SDL_Log(SDL_GetGPUDriver(1));

	SDL_Log("Attaching window");
	if (!SDL_ClaimWindowForGPUDevice(sdl_device, sdl_window))
	{
		const char* error = SDL_GetError();
		SDL_Log(SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_ShowWindow(sdl_window);


	get_layer_stack().add_layer(new WorldLayer("world"));
	get_layer_stack().add_layer(new GUILayer("gui"));

	for (auto layer : get_layer_stack())
	{
		layer->init();
	}

	return SDL_APP_CONTINUE;
}