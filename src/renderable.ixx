module;

#include <vector>
#include <glm/glm.hpp>

export module renderable;

import vertex;

export class Renderable
{
public:
	std::vector<glm::vec2> core;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	int vertex_offset = 0;
	int index_offset = 0;

	glm::vec3 color;

	Renderable()
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

	inline int get_vertex_offset()
	{
		return vertex_offset;
	}

	inline void set_buffer_offset(int offset)
	{
		vertex_offset = offset;
	}

	inline int get_index_offset()
	{
		return index_offset;
	}

	inline void set_index_offset(int offset)
	{
		index_offset = offset;
	}

	virtual void generate_indices() = 0;
	virtual void generate_vertices() = 0;	
};
