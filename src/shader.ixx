module;

#include <fstream>
#include <iostream>
#include <SDL3/SDL.h>
#include <sstream>
#include <string>

export module shader;

import sdl_data;

export SDL_GPUShader* load_shader(
	SDL_GPUDevice* device,
	const char* shader_base_name,
	Uint32 sample_count,
	Uint32 uniform_buffer_count,
	Uint32 storage_buffer_count,
	Uint32 storage_texture_count
) {

	SDL_GPUShaderStage stage;
	if (SDL_strstr(shader_base_name, "_vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(shader_base_name, "_frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		SDL_Log("Invalid shader stage!");
		return NULL;
	}

	char fullPath[256];
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint;


	SDL_snprintf(fullPath, sizeof(fullPath), "shaders/compiled/%s.spv", shader_base_name);
	format = SDL_GPU_SHADERFORMAT_SPIRV;
	entrypoint = "main";


	size_t codeSize;
	void* code = SDL_LoadFile(fullPath, &codeSize);
	if (code == NULL)
	{
		SDL_Log("Failed to load shader from disk! %s", fullPath);
		return NULL;
	}

	SDL_GPUShaderCreateInfo shader_info = {};
	shader_info.code = (const Uint8*)code;
	shader_info.code_size = codeSize;
	shader_info.entrypoint = entrypoint;
	shader_info.format = format;
	shader_info.stage = stage;
	shader_info.num_samplers = sample_count;
	shader_info.num_uniform_buffers = uniform_buffer_count;
	shader_info.num_storage_buffers = storage_buffer_count;
	shader_info.num_storage_textures = storage_texture_count;

	SDL_GPUShader* shader = SDL_CreateGPUShader(sdl_device, &shader_info);
	if (shader == NULL)
	{
		SDL_Log("Failed to create shader!");
		SDL_free(code);
		return NULL;
	}

	SDL_free(code);
	return shader;
}

