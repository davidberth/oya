module;

#include <string>
#include <functional>
#include "loguru.hpp"
#include "GL/glew.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

export module world_layer;

import layer;
import gui_data;
import keyboard_data;
import window_data;
import data_trigger;
import mouse_data;
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
		add_listener(&mouse_button_data, &WorldLayer::on_mouse_button);
		add_listener(&function_keyboard_data, &WorldLayer::on_function_keyboard);

		shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
		viewLoc = glGetUniformLocation(shader->programID, "view_proj");

		// set up the geometry for the scene
		glm::vec2 vertices[] = {
			glm::vec2{-0.5f, -0.5f},
			glm::vec2{0.5f, -0.5f},
			glm::vec2{0.5f, 0.5f},
			glm::vec2{-0.5f, 0.5f}
		};
		
		scene.add_node(vertices, 4, 1.0f, 0.0f, 0.0f, 0.1f);

		vertices[0].x += 2.0f;
		vertices[1].x += 2.0f;
		vertices[2].x += 2.0f;
		vertices[3].x += 2.0f;
		
		scene.add_node(vertices, 4, 0.0f, 0.0f, 1.0f, 0.1f);
		scene.setup();
  
		
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

		glClearColor(gui_data.clear_color[0], 
					 gui_data.clear_color[1],
					 gui_data.clear_color[2],
					 gui_data.clear_color[3]);
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

	// listeners
	void on_mouse_button()
	{
		
		LOG_F(INFO, "On Mouse Called!");
		mouse_button_data.log();
	}

    void on_function_keyboard()
    {
        if (function_keyboard_data.F10_down)
        {
            remove_fbo();
			LOG_F(INFO, "window width %d, window height %d", window_data.width, window_data.height);
            glViewport(0, 0, window_data.width, window_data.height);
			persistent_data.gui_on = false;
            
        }
        else if (function_keyboard_data.F11_down)
        {
            add_fbo();
			persistent_data.gui_on = true;
        }
    }
};

