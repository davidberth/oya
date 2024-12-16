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
import scene_loader;
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
		
	};

	~Scene() {
		for (int i = 0; i < root->children.size(); i++)
		{
			delete root->children.at(i);
		}
		delete root;
		delete shader;
	};

	void add_node(glm::vec2 vertices[], int num_vertices, float outline_width,
		float red, float green, float blue,
		float outline_red, float outline_green, float outline_blue)
	{
		Node* node = new Node();
		for (int i = 0; i < num_vertices; i++)
		{
			node->add_vertex(vertices[i]);
		}
		node->outline_width = outline_width;
		node->color = glm::vec3(red, green, blue);
		node->outline_color = glm::vec3(outline_red, outline_green, outline_blue);

		generate_indices(node);
		
		add_outline(node, outline_width);
		generate_outline_indices(node);
		generate_vertices(node);
		root->children.push_back(node);
	}

	void setup()
	{
		geom.clear_buffer();
		for (int i = 0; i < root->children.size(); i++)
		{
			geom.add_node(root->children.at(i));
		}
		geom.setup_vbo();
		shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
		viewLoc = glGetUniformLocation(shader->programID, "view_proj_model");
	}

	void render(const glm::mat4 view_proj)
	{
		shader->use();

		root->set_rotation(12.f);
		render_node(root, glm::mat4(1.0f), view_proj);
	}

	void render_node(Node* node, const glm::mat4& parent_transform, const glm::mat4& view_proj)
	{
		glm::mat4 global_transform = parent_transform * node->get_transform();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_proj * parent_transform));
		geom.render(node->get_index_offset(), node->get_num_indices(), view_proj, global_transform);

		for (Node* child : node->children)
		{
			render_node(child, global_transform, view_proj);
		}
	}

	void load_from_file(const std::string& filename) {
		SceneLoader loader;
		try {
			auto nodes = loader.loadFromFile(filename);

			// recursive function to process nodes
			std::function<void(const std::shared_ptr<SceneLoader::NodeData>&)> processNode =
				[&](const std::shared_ptr<SceneLoader::NodeData>& node) {
				// add current node to scene
				if (node->type == "polygon" && !node->vertices.empty()) {
					add_node(
						node->vertices.data(),           // vertex array
						node->vertices.size(),           // vertex count
						node->outlineWidth,              // outline width
						node->fillColor.r,               // fill color r
						node->fillColor.g,               // fill color g
						node->fillColor.b,               // fill color b
						node->outlineColor.r,            // outline color r
						node->outlineColor.g,            // outline color g
						node->outlineColor.b             // outline color b
					);
				}

				// process all children
				for (const auto& child : node->children) {
					processNode(child);
				}
				};

			// process all root nodes
			for (const auto& node : nodes) {
				processNode(node);
			}

			setup();

		}
		catch (const std::exception& e) {
			LOG_F(ERROR, "Failed to load scene from %s: %s", filename.c_str(), e.what());
			throw;
		}
	}
};