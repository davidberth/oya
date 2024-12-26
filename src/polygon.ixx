module;

#include <vector>
#include <glm/glm.hpp>

export module polygon;

import vertex;

bool point_in_triangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
	float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
	float s = 1.0f / (2.0f * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y);
	float t = 1.0f / (2.0f * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y);
	return s >= 0.0f && t >= 0.0f && (1.0f - s - t) >= 0.0f;
}


bool is_valid_ear(std::vector<glm::vec2>& position, size_t prev, size_t curr,
	size_t next, const std::vector<unsigned int>& remaining_vertices) {
	glm::vec2 a = position[remaining_vertices[prev]];
	glm::vec2 b = position[remaining_vertices[curr]];
	glm::vec2 c = position[remaining_vertices[next]];

	// Check if vertex forms a convex angle
	glm::vec2 ab = b - a;
	glm::vec2 bc = c - b;
	float cross_product = ab.x * bc.y - ab.y * bc.x;
	if (cross_product <= 0.0f) return false;  // Not convex

	// Check if any other vertex lies inside this triangle
	for (size_t i = 0; i < remaining_vertices.size(); i++) {
		if (i == prev || i == curr || i == next) continue;

		if (point_in_triangle(position[remaining_vertices[i]], a, b, c)) {
			return false;  // Another vertex found inside
		}
	}
	return true;
}

export class Polygon
{
public:
	std::vector<glm::vec2> outline;
	std::vector<glm::vec2> buffer;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int buffer_offset = 0;
	int index_offset = 0;

	glm::vec3 color;
	glm::vec3 outline_color = { 0.1f, 0.1f, 0.1f };
	float outline_width;

	Polygon()
	{
		vertices.clear();
		indices.clear();
	}

	void setup()
	{
		generate_indices();
		add_outline();
		generate_outline_indices();
		generate_vertices();
		compute_centroid();

	}

	void add_vertex(glm::vec2 pos)
	{
		outline.push_back(pos);
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
		for (int i = 0; i < outline.size(); i++)
		{
			centroid += outline.at(i);
		}
		if (outline.size() > 0) centroid /= outline.size();
		return centroid;
	}

	inline int get_buffer_offset()
	{
		return buffer_offset;
	}

	inline void set_buffer_offset(int offset)
	{
		buffer_offset = offset;
	}

	inline int get_index_offset()
	{
		return index_offset;
	}

	inline void set_index_offset(int offset)
	{
		index_offset = offset;
	}

	void add_outline()
	{

		size_t vertex_count = outline.size();
		std::vector<glm::vec2> offset_vertices(vertex_count);

		for (size_t i = 0; i < vertex_count; i++) {
			size_t prev = (i + vertex_count - 1) % vertex_count;
			size_t next = (i + 1) % vertex_count;

			// Get vectors to previous and next vertices
			glm::vec2 to_prev = glm::normalize(outline[prev] - outline[i]);
			glm::vec2 to_next = glm::normalize(outline[next] - outline[i]);

			// Calculate bisector vector (perpendicular to angle bisector)
			glm::vec2 bisector = glm::normalize(to_prev + to_next);

			// Calculate the angle between edges
			float sin_half_angle = glm::length(glm::cross(glm::vec3(to_next, 0), glm::vec3(to_prev, 0))) / 2.0f;

			// Calculate offset distance (adjusts for angle to maintain consistent visual width)
			float offset_distance = outline_width / sin_half_angle;

			// Calculate offset vertex
			buffer.push_back(outline[i] - bisector * offset_distance);
		}
	}

	void generate_outline_indices()
	{
		size_t vertex_count = outline.size();
		size_t vertex_count2 = vertex_count * 2;
		for (size_t i = 0; i < vertex_count; i++) {
			size_t next = (i + 1) % vertex_count;

			// Get indices for current quad
			unsigned int curr_inner = i + vertex_count;
			unsigned int curr_outer = i + vertex_count2;
			unsigned int next_inner = next + vertex_count;
			unsigned int next_outer = next + vertex_count2;

			// First triangle
			indices.push_back(curr_inner);
			indices.push_back(curr_outer);
			indices.push_back(next_inner);

			// Second triangle
			indices.push_back(curr_outer);
			indices.push_back(next_outer);
			indices.push_back(next_inner);
		}
	}

	void generate_indices()
	{

		if (outline.size() < 3) return;
		indices.clear();

		// Create a list of vertex indices that we'll process
		std::vector<unsigned int> remaining_vertices;
		for (size_t i = 0; i < outline.size(); i++) {
			remaining_vertices.push_back(i);
		}

		// Process until we have a triangle
		while (remaining_vertices.size() > 3) {
			bool ear_found = false;

			// Look for an ear
			for (unsigned int i = 0; i < remaining_vertices.size(); i++) {
				unsigned int prev = (i + remaining_vertices.size() - 1) % remaining_vertices.size();
				unsigned int next = (i + 1) % remaining_vertices.size();

				if (is_valid_ear(outline, prev, i, next, remaining_vertices)) {
					// Add triangle to indices
					indices.push_back(remaining_vertices[prev]);
					indices.push_back(remaining_vertices[i]);
					indices.push_back(remaining_vertices[next]);

					// Remove the middle vertex
					remaining_vertices.erase(remaining_vertices.begin() + i);
					ear_found = true;
					break;
				}
			}

			// If no ear is found (shouldn't happen with valid polygons)
			if (!ear_found) {
				break;
			}
		}

		// Add the final triangle
		if (remaining_vertices.size() == 3) {
			indices.push_back(remaining_vertices[0]);
			indices.push_back(remaining_vertices[1]);
			indices.push_back(remaining_vertices[2]);
		}
	}

	void generate_vertices()
	{
		vertices.clear();
		vertices.reserve(outline.size() * 2 + buffer.size());
		for (const auto& point : outline)
		{
			vertices.push_back(Vertex{ point.x, point.y, color.x, color.y, color.z });
		}
		float redo = outline_color.x * 0.9f;
		float greeno = outline_color.y * 0.9f;
		float blueo = outline_color.z * 0.9f;
		for (const auto& point : outline)
		{
			vertices.push_back(Vertex{ point.x, point.y, redo, greeno, blueo });
		}

		for (const auto& point : buffer)
		{
			vertices.push_back(Vertex{ point.x, point.y, outline_color.x, outline_color.y, outline_color.z });
		}
	}
};