module;

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "loguru.hpp"

export module geometry_buffer;

import polygon;
import vertex;
import render_stats_event;
import event;


export class GeometryBuffer
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int vertex_buffer_size = 40000;
	int index_buffer_size = 40000;
	int current_offset = 0;
	int index_offset = 0;

	int num_draw_calls = 0;
	int num_total_indices = 0;

	GLuint VBO = -1;
	GLuint VAO = -1;
	GLuint EBO = -1;

	bool dynamic = false;
public:
	GeometryBuffer(bool dyn) { dynamic = dyn; };
	~GeometryBuffer() {};

	void clear_buffer()
	{
		vertices.clear();
		indices.clear();
		current_offset = 0;
		index_offset = 0;
	}

	void add_polygon(Polygon *polygon)
	{
		polygon->set_buffer_offset(current_offset);
		polygon->set_index_offset(index_offset);

		for (int i = 0; i < polygon->get_num_vertices(); i++)
		{
			vertices.push_back(polygon->vertices.at(i));
		}
		for (int i = 0; i < polygon->get_num_indices(); i++)
		{
			indices.push_back(polygon->indices.at(i) + current_offset);
		}
		current_offset += polygon->get_num_vertices();
		index_offset += polygon->get_num_indices();
	}

	int get_num_vertices()
	{
		return vertices.size();
	}

	int get_num_indices()
	{
		return indices.size();
	}

	void setup_vbo()
	{

		GLuint draw_type = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

		if (VAO >= 0)
		{
			glDeleteVertexArrays(1, &VAO);
		}
		if (VBO >= 0)
		{
			glDeleteBuffers(1, &VBO);
		}
		if (EBO >= 0)
		{
			glDeleteBuffers(1, &EBO);
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), draw_type);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), draw_type);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, red));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void update_vbo()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
	}

	void render(int start_index, int num_indices) 
	{
		if (num_indices > 0)
		{
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void*)(start_index * sizeof(unsigned int)));
			
			num_draw_calls += 1;
			num_total_indices += num_indices;
		}
	}

	void new_frame()
	{
		glBindVertexArray(VAO);
		num_draw_calls = 0;
		num_total_indices = 0;
	}

	void end_frame()
	{
		glBindVertexArray(0);
		if (num_draw_calls == 0)
		{
			event_dispatcher.dispatch(RenderStatsEvent(0, 0.0f, 0.0f));
			return;
		}
		float average_indices = num_total_indices / num_draw_calls;
		float average_triangles = average_indices / 3;
		
		event_dispatcher.dispatch(RenderStatsEvent(num_draw_calls, average_indices, average_triangles));
	}
};