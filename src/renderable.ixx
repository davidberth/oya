module;

#include <glm/glm.hpp>
#include <vector>


export module renderable;

import vertex;


unsigned long int global_id_counter(1);

export class Renderable
{
public:
	unsigned long int id;
	std::vector<glm::vec2> core;
	glm::vec3 color;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Renderable() : id(global_id_counter++)
	{
		vertices.clear();
		indices.clear();
	}

	void setup()
	{
		generate_vertices();
		generate_indices();
		compute_centroid();
	}

	void add_vertex(glm::vec2 pos)
	{
		core.push_back(pos);
	}

	int get_num_vertices()
	{
		return vertices.size();
	}

	int get_num_indices()
	{
		return indices.size();
	}

	glm::vec2 compute_centroid()
	{
		glm::vec2 centroid = glm::vec2(0.0f, 0.0f);
		for (int i = 0; i < core.size(); i++)
		{
			centroid += core.at(i);
		}
		if (core.size() > 0) centroid /= core.size();
		return centroid;
	}

	virtual void generate_indices() = 0;
	virtual void generate_vertices() = 0;
};
