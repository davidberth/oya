module;

#include <SDL3/SDL.h>

export module render;

import layer_stack;
import sdl_data;


export void render()
{
	sdl_cmdbuf = SDL_AcquireGPUCommandBuffer(sdl_device);
	if (sdl_cmdbuf == NULL)
	{
		SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
	}

	if (!SDL_AcquireGPUSwapchainTexture(sdl_cmdbuf, sdl_window, &sdl_swapchain_texture, NULL, NULL)) {
		SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
	}


	// render
	for (auto layer : *get_layer_stack())
	{
		if (layer->do_render)
		{
			layer->begin();
			layer->render();
			layer->end();
		}

	}

	SDL_SubmitGPUCommandBuffer(sdl_cmdbuf);

};