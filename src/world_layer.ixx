module;

#include <string>
#include <functional>
#include "loguru.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

export module world_layer;

import layer;
import gui_data;
import data_listener;
import mouse_data;

// Vertex data
GLfloat vertices[] = {
	// Positions      // Colors
	0.0f,  0.5f,      1.0f, 0.0f, 0.0f,  // Top vertex (Red)
   -0.5f, -0.5f,      0.0f, 1.0f, 0.0f,  // Bottom left vertex (Green)
	0.5f, -0.5f,      0.0f, 0.0f, 1.0f   // Bottom right vertex (Blue)
};

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    ourColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)";


export class WorldLayer : public Layer
{
    GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	GLuint VBO, VAO;

public:
	WorldLayer(std::string pname) : Layer(pname) {};
	~WorldLayer() {};
	virtual void init(GLFWwindow* window) override
	{
		Layer::init(window);
		add_listener(&mouse_button_data, &WorldLayer::on_mouse_button);

        // Build and compile the shader program
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

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
		// glViewport(0, 0, display_w, display_h);
		glClearColor(gui_data.clear_color[0], 
					 gui_data.clear_color[1],
					 gui_data.clear_color[2],
					 gui_data.clear_color[3]);
		glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
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
	}

	// listeners
	void on_mouse_button()
	{
		
		LOG_F(INFO, "On Mouse Called!");
		mouse_button_data.log();
	}

};