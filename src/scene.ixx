module;

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <string>


export module scene;

import node;
import render_stats_event;
import outline;
import render_set;
import shader;
import sdl_data;

export class Scene
{
public:
	Node* root;
	RenderSet* render_set = nullptr;

	Scene() {

		root = new Node();
		root->centroid = glm::vec2(0.0f, 0.0f);
		root->rotate_delta = 0.001f;

		SDL_GPUShader* vertex_shader = load_shader(sdl_device, "polygon_vert", 0, 1, 0, 0);
		SDL_GPUShader* fragment_shader = load_shader(sdl_device, "polygon_frag", 0, 0, 0, 0);

		render_set = new RenderSet();
		render_set->init(RenderGeometryType::TRIANGLE_LIST, vertex_shader, fragment_shader, 100000, 100000);
	};

	~Scene() {
		for (int i = 0; i < root->children.size(); i++)
		{
			delete root->children.at(i);
		}
		delete root;
		delete render_set;
	};

	Node* add_node(Node* parent, glm::vec2 vertices[], int num_vertices,
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
		node->compute_local_aabb();
		parent->children.push_back(node);
		return node;
	}

	void setup()
	{
		check_transform_needs(root);
		propagate_transform_needs(root);
		count_total_indices(root);
		add_node_recursive(root);
	}

	void stage_renderables()
	{
		render_set->begin();
		stage_node(root);
		render_set->end();
	}

	void stage_node(Node* node)
	{
		render_set->add(&node->polygon);
		for (Node* child : node->children) {
			stage_node(child);
		}
	}

	void render(SDL_GPURenderPass* render_pass, const glm::mat4& view_proj)
	{
		render_set->render_all_geometries(render_pass, view_proj);
	}

	void render_node(Node* node, const glm::mat4& parent_transform, const glm::mat4& view_proj)
	{
		// glm::mat4 global_transform = parent_transform * node->get_transform();

		/*
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
		*/
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
		//geom->add_renderable(&node->polygon);
		if (node->local_aabb.outline == nullptr) {
			node->local_aabb.outline = new Outline();
			node->local_aabb.outline->color = glm::vec3(0.6f, 0.6f, 0.6f);
			node->local_aabb.outline->core = {
				glm::vec2(node->local_aabb.min_.x, node->local_aabb.min_.y),
				glm::vec2(node->local_aabb.min_.x, node->local_aabb.max_.y),
				glm::vec2(node->local_aabb.max_.x, node->local_aabb.max_.y),
				glm::vec2(node->local_aabb.max_.x, node->local_aabb.min_.y)
			};
			node->local_aabb.outline->generate_indices();
			node->local_aabb.outline->generate_vertices();
		}
		//debug_geom->add_renderable(node->local_aabb.outline);
		for (Node* child : node->children) {
			add_node_recursive(child);
		}
	}
};

