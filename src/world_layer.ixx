module;

#include <string>
#include <functional>
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <iostream>


export module world_layer;

import event;
import key_event;
import layer;

import camera;
import persistent_data;
import geometry_renderer;
import scene;
import scene_event;
import scene_serializer;

export class WorldLayer : public Layer
{
  
	Scene scene;

public:
	WorldLayer(std::string pname) : Layer(pname) {};
	~WorldLayer() {};
	virtual void init(GLFWwindow* window) override
	{
		Layer::init(window);

		event_dispatcher.subscribe<SceneEvent>([this](const SceneEvent& scene_event) {
			if (scene_event.event_type == SceneEventType::load) {
				scene_serializer.deserialize(scene, scene_event.scene_path);
				scene.setup();
			}
			else if (scene_event.event_type == SceneEventType::save) {
				scene_serializer.serialize(scene, scene_event.scene_path);
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

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		scene.render(camera.view_proj);
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

