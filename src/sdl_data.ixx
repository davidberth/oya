module;

#include <SDL3/SDL.h>

export module sdl_data;

export SDL_Window* sdl_window = nullptr;
export SDL_GPUDevice* sdl_device = nullptr;
export SDL_Renderer* sdl_renderer = nullptr;
export SDL_GPUCommandBuffer* sdl_cmdbuf = nullptr;
export SDL_GPUTexture* sdl_swapchain_texture = nullptr;
