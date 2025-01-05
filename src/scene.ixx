module;

#include <glm/glm.hpp>
#include <string>
#include "GL/glew.h"

export module scene;

import node;
import geometry_renderer;
import render_stats_event;
import outline;


export class Scene
{
public:
    Node* root;
    GeometryRenderer *geom;
    GeometryRenderer *debug_geom;

    std::vector<Outline *> test_outlines;

    Scene() {
      
        root = new Node();
        root->centroid = glm::vec2(0.0f, 0.0f);
        root->rotate_delta = 0.001f;

        geom = new GeometryRenderer(false, RenderType::polygon, RenderStatsEventSet::world_geometry);
        debug_geom = new GeometryRenderer(true, RenderType::line, RenderStatsEventSet::debug_render);

        test_outlines.clear();
        Outline* outline1 = new Outline();
		outline1->color = glm::vec3(1.0f, 1.0f, 1.0f);
		outline1->core.push_back(glm::vec2(-0.5f, -0.5f));
		outline1->core.push_back(glm::vec2(0.5f, -0.5f));
		outline1->core.push_back(glm::vec2(0.5f, 0.5f));
		outline1->core.push_back(glm::vec2(-0.5f, 0.5f));
        outline1->setup();
		test_outlines.push_back(outline1);

		Outline* outline2 = new Outline();
		outline2->color = glm::vec3(0.5f, 1.0f, 0.5f);
		outline2->core.push_back(glm::vec2(0.6f, -0.5f));
		outline2->core.push_back(glm::vec2(1.6f, -0.5f));
		outline2->core.push_back(glm::vec2(1.6f, 0.5f));
		outline2->core.push_back(glm::vec2(0.6f, 0.5f));
        outline2->setup();
		test_outlines.push_back(outline2);
    };

    ~Scene() {
        for (int i = 0; i < root->children.size(); i++)
        {
            delete root->children.at(i);
        }

		for (Outline *outline : test_outlines)
		{
			delete outline;
		}

        delete geom;
        delete debug_geom;
        delete root;
     
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
        geom->clear_buffer();
        debug_geom->clear_buffer();

        check_transform_needs(root);
        propagate_transform_needs(root);
        count_total_indices(root);
        add_node_recursive(root);
        add_outlines();

        geom->setup_vbo();
        debug_geom->setup_vbo();

		geom->assign_shader("shaders/polygon_vert.glsl", "shaders/polygon_frag.glsl");
		debug_geom->assign_shader("shaders/polygon_vert.glsl", "shaders/polygon_frag.glsl");

    }

    void render(const glm::mat4 &view_proj)
    {
        geom->new_frame();
        render_node(root, glm::mat4(1.0f), view_proj);
        geom->end_frame();

		debug_geom->new_frame();
        debug_geom->set_transformation(view_proj);
		debug_geom->render(0, debug_geom->get_num_indices());
		debug_geom->end_frame();
    }

    void render_node(Node* node, const glm::mat4& parent_transform, const glm::mat4& view_proj)
    {
        glm::mat4 global_transform = parent_transform * node->get_transform();
        geom->set_transformation(view_proj * global_transform);
      
        if (!node->needs_child_transforms) {
            // render this node and all children in one draw call
            geom->render(node->polygon.get_index_offset(), node->total_indices);
        }
        else {
            // render just this node
            geom->render(node->polygon.get_index_offset(), node->polygon.get_num_indices());

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

	void add_outlines()
	{
		for (Outline* outline : test_outlines)
		{
			debug_geom->add_renderable(outline);
		}
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
        geom->add_renderable(&node->polygon);
        for (Node* child : node->children) {
            add_node_recursive(child);
        }
    }
};

