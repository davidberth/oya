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
import vertex;
import textured_quad;
import font_manager;


export class Scene
{
public:
	Node* root;
	RenderSet<Vertex>* world_render_set = nullptr;
	RenderSet<TextureVertex>* font_render_set = nullptr;

	glm::mat4x4 identity = glm::mat4(1.0f);

	TexturedQuad font_quad;
	FontManager font_manager;

	Scene() {

		font_quad.add_vertex(glm::vec2(-0.8f, -0.8f));
		font_quad.add_vertex(glm::vec2(0.8f, 0.8f));
		font_quad.setup();

		root = new Node();
		root->centroid = glm::vec2(0.0f, 0.0f);
		root->rotate_delta = 0.001f;

		font_manager.load_font("font");


		/*SDL_Surface* font_surface = SDL_LoadBMP("resources/textures/font.bmp");
		if (font_surface == NULL)
		{
			SDL_Log("Could not load font texture!");
		}*/

		SDL_GPUShader* vertex_shader_world = load_shader(sdl_device, "polygon_vert", 0, 1, 0, 0);
		SDL_GPUShader* fragment_shader_world = load_shader(sdl_device, "polygon_frag", 0, 0, 0, 0);

		world_render_set = new RenderSet<Vertex>();
		world_render_set->init(RenderGeometryType::TRIANGLE_LIST, 5000,
			vertex_shader_world, fragment_shader_world, 100000, 100000,
			nullptr);


	};

	~Scene() {
		for (int i = 0; i < root->children.size(); i++)
		{
			delete root->children.at(i);
		}
		delete root;
		delete world_render_set;
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
		world_render_set->begin();
		stage_node(root);
		world_render_set->end();

		font_render_set->begin();
		font_render_set->add(&font_quad, &identity);
		font_render_set->end();
	}

	void stage_node(Node* node)
	{
		if (node->polygon.get_num_indices() > 0)
		{
			if (node->needs_transform) {
				world_render_set->add(&node->polygon, &node->world_transform);
			}
			else {
				world_render_set->add(&node->polygon, nullptr);
			}
		}

		for (Node* child : node->children) {
			stage_node(child);
		}
	}

	void render(SDL_GPURenderPass* render_pass, const glm::mat4& view_proj) const
	{
		world_render_set->render_all_geometries(render_pass, view_proj);
		font_render_set->render_all_geometries(render_pass, identity);
	}


	void compute_world_transform(Node* node, const glm::mat4& parent_transform) {
		if (node->needs_transform) {
			node->world_transform = parent_transform * node->local_transform;
		}
		else {
			node->world_transform = parent_transform;
		}

		for (Node* child : node->children) {
			compute_world_transform(child, node->world_transform);
		}
	}

	void compute_world_transforms() {
		compute_world_transform(root, glm::mat4(1.0f));
	}

	void update(float dt)
	{
		root->update(dt);
	}


private:
	void check_transform_needs(Node* node) {
		constexpr float epsilon = 0.00001f;
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

		for (Node* child : node->children) {
			add_node_recursive(child);
		}
	}
};

