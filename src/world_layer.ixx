module;

#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <SDL3/SDL.h>
#include <string>

export module world_layer;

import event;
import key_event;
import layer;

import camera;
import persistent_data;
import scene;
import scene_event;
import scene_serializer;
import sdl_data;

export class WorldLayer : public Layer
{
	Scene scene;

public:
	WorldLayer(std::string pname) : Layer(pname) {}
	virtual ~WorldLayer() {
	}
	virtual void init() override
	{
		Layer::init();

		get_event_dispatcher()->subscribe<SceneEvent>([this](const SceneEvent& scene_event) {
			if (scene_event.event_type == 0) {
				get_scene_serializer()->deserialize(scene, "scenes/scene1.scn");
				scene.setup();
			}
			else if (scene_event.event_type == 1) {
				get_scene_serializer()->serialize(scene, "scenes/scene1.scn");
			}
			});

	}
	virtual void update() override
	{
		scene.update(0.01f);
	}
	virtual void begin()
	{
		Layer::begin();
	}
	virtual void render() override
	{
		if (sdl_swapchain_texture != NULL)
		{
			scene.stage_renderables();

			SDL_GPUColorTargetInfo color_target_info = { 0 };
			color_target_info.texture = sdl_swapchain_texture;
			color_target_info.clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
			color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
			color_target_info.store_op = SDL_GPU_STOREOP_STORE;

			SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(sdl_cmdbuf, &color_target_info, 1, NULL);

			scene.render(render_pass, get_camera()->view_proj);

			SDL_EndGPURenderPass(render_pass);
		}
	}
	virtual void end() override
	{
		Layer::end();
	}
	virtual void cleanup() override
	{
		scene.render_set->cleanup();
		Layer::cleanup();

	}

};

