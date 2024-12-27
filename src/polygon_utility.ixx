module;

#include <vector>
#include <glm/glm.hpp>

export module polygon_utility;

export bool point_in_triangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
	float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
	float s = 1.0f / (2.0f * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y);
	float t = 1.0f / (2.0f * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y);
	return s >= 0.0f && t >= 0.0f && (1.0f - s - t) >= 0.0f;
}


export bool is_valid_ear(std::vector<glm::vec2>& position, size_t prev, size_t curr,
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