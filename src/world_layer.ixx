module;

#include <string>
#include <functional>
#include "loguru.hpp"
#include "GL/glew.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

export module world_layer;

import event;
import key_event;
import layer;

import shader;
import camera;
import persistent_data;
import geometry_buffer;
import scene;



export class WorldLayer : public Layer
{
    Shader *shader;
	GLuint viewLoc;
	Scene scene;

public:
	WorldLayer(std::string pname) : Layer(pname) {};
	~WorldLayer() {};
	virtual void init(GLFWwindow* window) override
	{
		Layer::init(window);

		
		shader = new Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
		viewLoc = glGetUniformLocation(shader->programID, "view_proj");

		scene.load_from_file("../scenes/test.scn");
		
		
	}
	virtual void update() override
	{

	}
	virtual void begin()
	{
		Layer::begin();
	}
	virtual void render() override
	{

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view_proj));
		scene.render();

        
	}
	virtual void end() override
	{
		Layer::end();
	}
	virtual void cleanup() override
	{
		Layer::cleanup();
		delete shader;
	}

};

