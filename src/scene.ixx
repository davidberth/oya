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
		glm::vec3 outline_color)
	{
		Node* node = new Node();
		for (int i = 0; i < num_vertices; i++)
		{
			node->add_vertex(vertices[i]);
		}
		node->outline_width = outline_width;
		node->color = color;
		node->outline_color = outline_color;

		node->rotate_delta = 0.01f;

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
		std::function<void(Node*)> add_node_recursive = [&](Node* node) {
			geom.add_node(node);
			for (Node* child : node->children) {
				add_node_recursive(child);
			}
			};
		add_node_recursive(root);
		geom.setup_vbo();
		shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
		viewLoc = glGetUniformLocation(shader->programID, "view_proj_model");
	}

	void create_world()
	{
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
					glm::vec2(x - 1.4f, y),
					glm::vec2(x - 1.4f, y + 0.1f),
					glm::vec2(x - 1.3f, y + 0.1f)
				};

				Node *node = add_node(root, vertices, 4, 0.008f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				Node *node2 = add_node(node, vertices2, 4, 0.008f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				add_node(node2, vertices3, 4, 0.008f, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}

	}

	void render(const glm::mat4 view_proj)
	{
		shader->use();

		render_node(root, glm::mat4(1.0f), view_proj);
	}

	void render_node(Node* node, const glm::mat4& parent_transform, const glm::mat4& view_proj)
	{
		glm::mat4 global_transform = parent_transform * node->get_transform();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_proj * global_transform));
		geom.render(node->get_index_offset(), node->get_num_indices());

		for (Node* child : node->children)
		{
			render_node(child, global_transform, view_proj);
		}
	}

	void update(float dt)
	{
		root->update(dt);
	}

};