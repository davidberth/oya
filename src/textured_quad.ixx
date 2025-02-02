module;

#include <glm/glm.hpp>
#include <vector>

export module textured_quad;

import vertex;
import renderable;


export class TexturedQuad : public Renderable<TextureVertex>
{
public:

	glm::vec3 color;

	TexturedQuad() : Renderable()
	{
	}

	void generate_indices()
	{
		if (core.size() != 2) return;
		indices.clear();
		indices.reserve(6);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
	}

	void generate_vertices()
	{
		vertices.clear();
		vertices.reserve(core.size());

		vertices.push_back(TextureVertex{ core[0].x, core[0].y, 0.0f, 0.0f });
		vertices.push_back(TextureVertex{ core[1].x, core[0].y, 1.0f, 0.0f });
		vertices.push_back(TextureVertex{ core[1].x, core[1].y, 1.0f, 1.0f });
		vertices.push_back(TextureVertex{ core[0].x, core[1].y, 0.0f, 1.0f });

	}
};