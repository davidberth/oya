module;

#include <vector>
#include <glm/glm.hpp>

export module outline;

import vertex;
import polygon_utility;
import renderable;


export class Outline : public Renderable<Vertex>
{
public:

	glm::vec3 color;

	Outline() : Renderable()
	{
	}

	void generate_indices()
	{
		if (core.size() < 3) return;
		indices.clear();

		for (size_t i = 0; i < core.size(); ++i)
		{
			indices.push_back(i);
			indices.push_back((i + 1) % core.size());
		}
	}

	void generate_vertices()
	{
		vertices.clear();
		vertices.reserve(core.size());
		for (const auto& point : core)
		{
			vertices.push_back(Vertex{ point.x, point.y, color.x, color.y, color.z });
		}
	}
};