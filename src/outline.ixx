module;

#include <glm/glm.hpp>

export module outline;

import node;

static glm::vec2 calculate_miter(const glm::vec2& prev_dir, const glm::vec2& next_dir, float half_width) {
    glm::vec2 miter = glm::normalize(prev_dir + next_dir);
    float length = half_width / glm::dot(miter, glm::normalize(next_dir));
    return miter * length;
}

export void add_outline(Node* node)
{
    
    if (node->outline.size() < 2) return;
    int offset = node->vertices.size();
    float half_width = node->outline_width * 0.5f;
    size_t vertex_count = node->outline.size();

    std::vector<glm::vec2> directions;
    // pre-calculate segment directions
    for (size_t i = 0; i < vertex_count; i++) {
        size_t next_i = (i + 1) % vertex_count;
        glm::vec2 dir = glm::normalize(node->outline[next_i] - node->outline[i]);
        directions.push_back(dir);
    }

    // generate vertices for each point with miter joints
    for (size_t i = 0; i < vertex_count; i++) {
        size_t prev_i = (i + vertex_count - 1) % vertex_count;
        size_t next_i = (i + 1) % vertex_count;

        const glm::vec2& prev_dir = directions[prev_i];
        const glm::vec2& curr_dir = directions[i];

        // calculate miter vector
        glm::vec2 miter = calculate_miter(prev_dir, curr_dir, half_width);

        // add vertices for the miter joint
        glm::vec2 v0 = node->outline[i] + miter;
        glm::vec2 v1 = node->outline[i] - miter;

        node->vertices.push_back(Vertex{ v0.x, v0.y, node->outline_color.x, node->outline_color.y, node->outline_color.z });
        node->vertices.push_back(Vertex{ v1.x, v1.y, node->outline_color.x, node->outline_color.y, node->outline_color.z });
    }

    // generate indices for the triangles
	
    for (size_t i = 0; i < vertex_count; i++) {
        size_t next_i = (i + 1) % vertex_count + offset;
        size_t curr_pair = i * 2 + offset;
        size_t next_pair = next_i * 2 + offset;

        // first triangle
        node->indices.push_back(curr_pair);
        node->indices.push_back(curr_pair + 1);
        node->indices.push_back(next_pair);

        // second triangle
        node->indices.push_back(curr_pair + 1);
        node->indices.push_back(next_pair + 1);
        node->indices.push_back(next_pair);
    }
}

