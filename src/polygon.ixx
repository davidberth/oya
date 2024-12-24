module;

#include <glm/glm.hpp>

export module polygon;

import node;

static bool point_in_triangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
    float area = 0.5f * (-b.y * c.x + a.y * (-b.x + c.x) + a.x * (b.y - c.y) + b.x * c.y);
    float s = 1.0f / (2.0f * area) * (a.y * c.x - a.x * c.y + (c.y - a.y) * p.x + (a.x - c.x) * p.y);
    float t = 1.0f / (2.0f * area) * (a.x * b.y - a.y * b.x + (a.y - b.y) * p.x + (b.x - a.x) * p.y);
    return s >= 0.0f && t >= 0.0f && (1.0f - s - t) >= 0.0f;
}

// Helper function to check if an ear is valid (no other vertices inside it)
static bool is_valid_ear(std::vector<glm::vec2>& position, size_t prev, size_t curr, 
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

export void generate_indices(Node *node)
{

    if (node->outline.size() < 3) return;
    node->indices.clear();

    // Create a list of vertex indices that we'll process
    std::vector<unsigned int> remaining_vertices;
    for (size_t i = 0; i < node->outline.size(); i++) {
        remaining_vertices.push_back(i);
    }

    // Process until we have a triangle
    while (remaining_vertices.size() > 3) {
        bool ear_found = false;

        // Look for an ear
        for (unsigned int i = 0; i < remaining_vertices.size(); i++) {
            unsigned int prev = (i + remaining_vertices.size() - 1) % remaining_vertices.size();
            unsigned int next = (i + 1) % remaining_vertices.size();

            if (is_valid_ear(node->outline, prev, i, next, remaining_vertices)) {
                // Add triangle to indices
                node->indices.push_back(remaining_vertices[prev]);
                node->indices.push_back(remaining_vertices[i]);
                node->indices.push_back(remaining_vertices[next]);

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
        node->indices.push_back(remaining_vertices[0]);
        node->indices.push_back(remaining_vertices[1]);
        node->indices.push_back(remaining_vertices[2]);
    }
}

export void generate_vertices(Node *node)
{
    node->vertices.clear();
    node->vertices.reserve(node->outline.size() * 2 + node->buffer.size()); 
    for (const auto& point : node->outline)
    {
        node->vertices.push_back(Vertex{ point.x, point.y, node->color.x, node->color.y, node->color.z });
    }
    float redo = node->outline_color.x * 0.9f;
	float greeno = node->outline_color.y * 0.9f;
	float blueo = node->outline_color.z * 0.9f;
    for (const auto& point : node->outline)
    {
        node->vertices.push_back(Vertex{ point.x, point.y, redo, greeno, blueo });
    }


	for (const auto& point : node->buffer)
	{
		node->vertices.push_back(Vertex{ point.x, point.y, node->outline_color.x, node->outline_color.y, node->outline_color.z });
	}
}