module;

#include <SDL3/SDL.h>

export module cleanup;

import sdl_data;
import layer_stack;

export void cleanup()
{
	SDL_WaitForGPUIdle(sdl_device);

	for (auto layer : get_layer_stack())
	{
		layer->cleanup();
	}

	SDL_ReleaseWindowFromGPUDevice(sdl_device, sdl_window);
	SDL_DestroyGPUDevice(sdl_device);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}
