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
// import geometry_renderer;
import scene;
import scene_event;
import scene_serializer;
import sdl_data;

export class WorldLayer : public Layer
{
	Scene scene;

public:
	WorldLayer(std::string pname) : Layer(pname) {};
	~WorldLayer() {};
	virtual void init() override
	{
		Layer::init();

		get_event_dispatcher().subscribe<SceneEvent>([this](const SceneEvent& scene_event) {
			if (scene_event.event_type == SceneEventType::load) {
				get_scene_serializer().deserialize(scene, scene_event.scene_path);
				scene.setup();
			}
			else if (scene_event.event_type == SceneEventType::save) {
				get_scene_serializer().serialize(scene, scene_event.scene_path);
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
			SDL_GPUColorTargetInfo color_target_info = { 0 };
			color_target_info.texture = sdl_swapchain_texture;
			color_target_info.clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
			color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
			color_target_info.store_op = SDL_GPU_STOREOP_STORE;

			SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(sdl_cmdbuf, &color_target_info, 1, NULL);

			scene.render(render_pass, get_camera().view_proj);

			SDL_EndGPURenderPass(render_pass);
		}



	}
	virtual void end() override
	{
		Layer::end();
	}
	virtual void cleanup() override
	{
		Layer::cleanup();

	}

};

