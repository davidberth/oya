module;

#include <glm/glm.hpp>
#include <string>
#include <loguru.hpp>
#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>

export module scene;

import node;
import geometry_buffer;
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
        shader = nullptr;
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
            node->polygon.add_vertex(vertices[i]);
        }
        node->polygon.outline_width = outline_width;
        node->polygon.color = color;
        node->polygon.outline_color = outline_color;
        node->rotate_delta = rotate_delta;

        node->setup_polygon();
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
    }

    void setup_shader()
    {
        if (shader != nullptr)
        {
            delete shader;
        }
        shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
        viewLoc = glGetUniformLocation(shader->programID, "view_proj_model");
    }


    void render(const glm::mat4 view_proj)
    {
		if (shader == nullptr)
		{
			return;
		}
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
            geom.render(node->polygon.get_index_offset(), node->total_indices);
        }
        else {
            // render just this node
            geom.render(node->polygon.get_index_offset(), node->polygon.get_num_indices());

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
        int total = node->polygon.get_num_indices();
        for (Node* child : node->children) {
            total += count_total_indices(child);
        }
        node->total_indices = total;
        return total;
    }

    void add_node_recursive(Node* node) {
        geom.add_polygon(&node->polygon);
        for (Node* child : node->children) {
            add_node_recursive(child);
        }
    }
};

