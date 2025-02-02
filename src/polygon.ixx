module;

#include <vector>
#include <glm/glm.hpp>

export module polygon;

import vertex;
import polygon_utility;
import renderable;


export class Polygon : public Renderable<Vertex>
{
public:

	glm::vec3 color;

	glm::vec3 outline_color = { 0.1f, 0.1f, 0.1f };
	float outline_width;
	std::vector<glm::vec2> buffer;

	Polygon() : Renderable()
	{
	}

	void add_outline()
	{

		size_t vertex_count = core.size();
		std::vector<glm::vec2> offset_vertices(vertex_count);

		for (size_t i = 0; i < vertex_count; i++) {
			size_t prev = (i + vertex_count - 1) % vertex_count;
			size_t next = (i + 1) % vertex_count;

			// get vectors to previous and next vertices
			glm::vec2 to_prev = glm::normalize(core[prev] - core[i]);
			glm::vec2 to_next = glm::normalize(core[next] - core[i]);

			// calculate bisector vector (perpendicular to angle bisector)
			glm::vec2 bisector = glm::normalize(to_prev + to_next);

			// calculate the angle between edges
			float sin_half_angle = glm::length(glm::cross(glm::vec3(to_next, 0), glm::vec3(to_prev, 0))) / 2.0f;

			// calculate offset distance (adjusts for angle to maintain consistent visual width)
			float offset_distance = outline_width / sin_half_angle;

			// calculate offset vertex
			buffer.push_back(core[i] - bisector * offset_distance);
		}
	}

	void generate_outline_indices()
	{
		size_t vertex_count = core.size();
		size_t vertex_count2 = vertex_count * 2;
		for (size_t i = 0; i < vertex_count; i++) {
			size_t next = (i + 1) % vertex_count;

			// get indices for current quad
			unsigned int curr_inner = i + vertex_count;
			unsigned int curr_outer = i + vertex_count2;
			unsigned int next_inner = next + vertex_count;
			unsigned int next_outer = next + vertex_count2;

			// first triangle
			indices.push_back(curr_inner);
			indices.push_back(curr_outer);
			indices.push_back(next_inner);

			// second triangle
			indices.push_back(curr_outer);
			indices.push_back(next_outer);
			indices.push_back(next_inner);
		}
	}

	void generate_indices()
	{

		if (core.size() < 3) return;
		indices.clear();

		// create a list of vertex indices that we'll process
		std::vector<unsigned int> remaining_vertices;
		for (size_t i = 0; i < core.size(); i++) {
			remaining_vertices.push_back(i);
		}

		// process until we have a triangle
		while (remaining_vertices.size() > 3) {
			bool ear_found = false;

			// look for an ear
			for (unsigned int i = 0; i < remaining_vertices.size(); i++) {
				unsigned int prev = (i + remaining_vertices.size() - 1) % remaining_vertices.size();
				unsigned int next = (i + 1) % remaining_vertices.size();

				if (is_valid_ear(core, prev, i, next, remaining_vertices)) {
					// add triangle to indices
					indices.push_back(remaining_vertices[prev]);
					indices.push_back(remaining_vertices[i]);
					indices.push_back(remaining_vertices[next]);

					// remove the middle vertex
					remaining_vertices.erase(remaining_vertices.begin() + i);
					ear_found = true;
					break;
				}
			}

			// if no ear is found (shouldn't happen with valid polygons)
			if (!ear_found) {
				break;
			}
		}

		// add the final triangle
		if (remaining_vertices.size() == 3) {
			indices.push_back(remaining_vertices[0]);
			indices.push_back(remaining_vertices[1]);
			indices.push_back(remaining_vertices[2]);
		}

		generate_outline_indices();
	}

	void generate_vertices()
	{
		buffer.clear();
		add_outline();
		vertices.clear();
		vertices.reserve(core.size() * 2 + buffer.size());
		for (const auto& point : core)
		{
			vertices.push_back(Vertex{ point.x, point.y, color.x, color.y, color.z });
		}

		float redo = outline_color.x * 0.9f;
		float greeno = outline_color.y * 0.9f;
		float blueo = outline_color.z * 0.9f;

		for (const auto& point : core)
		{
			vertices.push_back(Vertex{ point.x, point.y, redo, greeno, blueo });
		}

		for (const auto& point : buffer)
		{
			vertices.push_back(Vertex{ point.x, point.y, outline_color.x, outline_color.y, outline_color.z });
		}
	}
};