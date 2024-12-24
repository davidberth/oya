module;

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

export module geometry_buffer;

import node;
import vertex;
import render_stats_event;
import event;


export class GeometryBuffer
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int vertex_buffer_size = 4000;
	int index_buffer_size = 4000;
	int current_offset = 0;
	int index_offset = 0;

	int num_draw_calls = 0;
	int num_total_indices = 0;

	GLuint VBO, VAO;
	GLuint EBO;
public:
	GeometryBuffer() {};
	~GeometryBuffer() {};

	void clear_buffer()
	{
		vertices.clear();
		indices.clear();
		current_offset = 0;
		index_offset = 0;

	}

	void add_node(Node* node)
	{
		node->set_buffer_offset(current_offset);
		node->set_index_offset(index_offset);

		for (int i = 0; i < node->get_num_vertices(); i++)
		{
			vertices.push_back(node->vertices.at(i));
		}
		for (int i = 0; i < node->get_num_indices(); i++)
		{
			indices.push_back(node->indices.at(i) + current_offset);
		}
		current_offset += node->get_num_vertices();
		index_offset += node->get_num_indices();
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

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, red));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
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
		float average_indices = num_total_indices / num_draw_calls;
		float average_triangles = average_indices / 3;
		
		event_dispatcher.dispatch(RenderStatsEvent(num_draw_calls, average_indices, average_triangles));
	}
};