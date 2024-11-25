module;

#include <glm/glm.hpp>

export module outline;

import node;

export void add_outline(Node* node, float outline_width)
{
    
    size_t vertex_count = node->outline.size();
    std::vector<glm::vec2> offset_vertices(vertex_count);

    for (size_t i = 0; i < vertex_count; i++) {
        size_t prev = (i + vertex_count - 1) % vertex_count;
        size_t next = (i + 1) % vertex_count;

        // Get vectors to previous and next vertices
        glm::vec2 to_prev = glm::normalize(node->outline[prev] - node->outline[i]);
        glm::vec2 to_next = glm::normalize(node->outline[next] - node->outline[i]);

        // Calculate bisector vector (perpendicular to angle bisector)
        glm::vec2 bisector = glm::normalize(to_prev + to_next);

        // Calculate the angle between edges
        float sin_half_angle = glm::length(glm::cross(glm::vec3(to_next, 0), glm::vec3(to_prev, 0))) / 2.0f;

        // Calculate offset distance (adjusts for angle to maintain consistent visual width)
        float offset_distance = outline_width / sin_half_angle;

        // Calculate offset vertex
        node->buffer.push_back(node->outline[i] - bisector * offset_distance);
    }  
}

export void generate_outline_indices(Node* node)
{
	size_t vertex_count = node->outline.size();
    size_t vertex_count2 = vertex_count * 2;
    for (size_t i = 0; i < vertex_count; i++) {
        size_t next = (i + 1) % vertex_count;

        // Get indices for current quad
        unsigned int curr_inner = i + vertex_count;
        unsigned int curr_outer = i + vertex_count2;
        unsigned int next_inner = next + vertex_count;
        unsigned int next_outer = next + vertex_count2;

        // First triangle
        node->indices.push_back(curr_inner);
        node->indices.push_back(curr_outer);
        node->indices.push_back(next_inner);

        // Second triangle
        node->indices.push_back(curr_outer);
        node->indices.push_back(next_outer);
        node->indices.push_back(next_inner);
    }
}