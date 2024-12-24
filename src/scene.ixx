module;

#include <glm/glm.hpp>
#include <string>
#include <loguru.hpp>
#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>

export module scene;

import node;
import geometry_buffer;
import polygon;
import outline;
import shader;


export class Scene
{
public:
    Node* root;
    GeometryBuffer geom;
    Shader* shader;
    GLuint viewLoc;

    Scene() {
        root = new Node();
        root->centroid = glm::vec2(0.0f, 0.0f);
        root->rotate_delta = 0.001f;
    };

    ~Scene() {
        for (int i = 0; i < root->children.size(); i++)
        {
            delete root->children.at(i);
        }
        delete root;
        delete shader;
    };

    Node* add_node(Node *parent, glm::vec2 vertices[], int num_vertices, 
        float outline_width,
        glm::vec3 color,
        glm::vec3 outline_color,
        float rotate_delta)
    {
        Node* node = new Node();
        for (int i = 0; i < num_vertices; i++)
        {
            node->add_vertex(vertices[i]);
        }
        node->outline_width = outline_width;
        node->color = color;
        node->outline_color = outline_color;

        node->rotate_delta = rotate_delta;

        generate_indices(node);
        
        add_outline(node, outline_width);
        generate_outline_indices(node);
        generate_vertices(node);
        node->compute_centroid();
        parent->children.push_back(node);
        return node;
    }

    void setup()
    {
        geom.clear_buffer();

        check_transform_needs(root);
        propagate_transform_needs(root);
        count_total_indices(root);
        add_node_recursive(root);

        geom.setup_vbo();
        shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
        viewLoc = glGetUniformLocation(shader->programID, "view_proj_model");
    }

    void create_world()
    {
		LOG_F(INFO, "Creating world");
        for (float x = -2.0; x < 2.1; x += 0.5)
        {
            for (float y = -2.0; y < 2.1; y += 0.5)
            {
                glm::vec2 vertices[] = {
                    glm::vec2(x, y),
                    glm::vec2(x + 0.1f, y),
                    glm::vec2(x + 0.1f, y + 0.1f),
                    glm::vec2(x, y + 0.1f)
                };

                glm::vec2 vertices2[] =
                {
                    glm::vec2(x + 1.3f, y),
                    glm::vec2(x + 1.4f, y),
                    glm::vec2(x + 1.4f, y + 0.1f),
                    glm::vec2(x + 1.3f, y + 0.1f)
                };

                glm::vec2 vertices3[] =
                {
                    glm::vec2(x - 1.3f, y),
                    glm::vec2(x - 1.2f, y),
                    glm::vec2(x - 1.2f, y + 0.1f),
                    glm::vec2(x - 1.3f, y + 0.1f)
                };

                Node *node = add_node(root, vertices, 4, 0.008f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.01f);
                Node *node2 = add_node(node, vertices2, 4, 0.008f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.01f);
                Node *node3 = add_node(node2, vertices3, 4, 0.008f, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);


                for (float xi = x - 0.5f; xi < x + 0.6f; xi += 0.5f)
                {
                    for (float yi = y - 0.5f; yi < y + 0.6f; yi += 0.5f)
                    {
                        glm::vec2 vertices4[] = {
                            glm::vec2(xi, yi),
                            glm::vec2(xi + 0.02f, yi),
                            glm::vec2(xi + 0.02f, yi + 0.1f),
                            glm::vec2(xi, yi + 0.1f)
                        };
                        Node* node4 = add_node(node3, vertices4, 4, 0.008f, glm::vec3(0.7f, 0.0f, 0.0f), glm::vec3(1.0f, 0.2f, 0.2f), 0.00f);
                    }
                }

            }
        }
		LOG_F(INFO, "World created");
    }

    void render(const glm::mat4 view_proj)
    {
        shader->use();
        geom.new_frame();
        render_node(root, glm::mat4(1.0f), view_proj);
        geom.end_frame();
    }

    void render_node(Node* node, const glm::mat4& parent_transform, const glm::mat4& view_proj)
    {
        glm::mat4 global_transform = parent_transform * node->get_transform();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_proj * global_transform));

        if (!node->needs_child_transforms) {
            // render this node and all children in one draw call
            geom.render(node->get_index_offset(), node->total_indices);
        }
        else {
            // render just this node
            geom.render(node->get_index_offset(), node->get_num_indices());

            // recursively render children that need transforms
            for (Node* child : node->children) {
                render_node(child, global_transform, view_proj);
            }
        }
    }

    void update(float dt)
    {
        root->update(dt);
    }

private:
    void check_transform_needs(Node* node) {
        constexpr float epsilon = 0.001f;
        node->needs_transform = std::abs(node->rotate_delta) > epsilon;
        for (Node* child : node->children) {
            check_transform_needs(child);
        }
    }

    bool propagate_transform_needs(Node* node) {
        bool needs_transforms = false;
        for (Node* child : node->children) {
            needs_transforms |= propagate_transform_needs(child);
        }
        node->needs_child_transforms = needs_transforms;
        return needs_transforms || node->needs_transform;
    }

    int count_total_indices(Node* node) {
        int total = node->get_num_indices();
        for (Node* child : node->children) {
            total += count_total_indices(child);
        }
        node->total_indices = total;
        return total;
    }

    void add_node_recursive(Node* node) {
        geom.add_node(node);
        for (Node* child : node->children) {
            add_node_recursive(child);
        }
    }
};

