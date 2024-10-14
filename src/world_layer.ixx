module;

#include <string>
#include <functional>
#include "loguru.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
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

// Vertex data
GLfloat vertices[] = {
	// Positions      // Colors
	0.0f,  0.5f,      1.0f, 0.0f, 0.0f,  // Top vertex (Red)
   -0.5f, -0.5f,      0.0f, 1.0f, 0.0f,  // Bottom left vertex (Green)
	0.5f, -0.5f,      0.0f, 0.0f, 1.0f   // Bottom right vertex (Blue)
};




export class WorldLayer : public Layer
{
    Shader *shader;
	GLuint VBO, VAO;
	GLuint viewLoc;

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

        // Set up vertex data and buffers and configure vertex attributes
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
		
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

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 vp = projection * view; 
		
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vp));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
            glViewport(0, 0, window_data.width, window_data.height);
            
        }
        else if (function_keyboard_data.F11_down)
        {
            add_fbo(0);
        }
    }
};