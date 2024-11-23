module;

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

export module geometry;

import node;
import vertex;


export class Geometry
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int vertex_buffer_size = 200;
	int index_buffer_size = 200;

	std::vector<Node> nodes;

	GLuint VBO, VAO;
	GLuint EBO;
public:
	Geometry() {};
	~Geometry() {};

	void add_node(const Vertex* lvertices, const unsigned int* lindices, int num_vertices, int num_indices)
	{
		// Store the current size of the vectors
		size_t cur_vert_pos = vertices.size();
		size_t cur_index_pos = indices.size();

		// Add new vertices and indices
		vertices.insert(vertices.end(), lvertices, lvertices + num_vertices);
		indices.insert(indices.end(), lindices, lindices + num_indices);

		// Add a new node with the current positions and sizes
		nodes.push_back(Node(&vertices[cur_vert_pos], &indices[cur_index_pos], num_vertices, num_indices));
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
		// Generate and bind the Vertex Array Object
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Generate and bind the Vertex Buffer Object
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Copy the vertex data to the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		// Generate and bind the Element Buffer Object

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		// Copy the index data to the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		// Define the vertex attributes
		// Position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glEnableVertexAttribArray(0);

		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		// Unbind the VAO
		glBindVertexArray(0);
	}

	void render()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
	}
};