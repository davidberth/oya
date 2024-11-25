module;

#include <glm/glm.hpp>

export module scene;

import node;
import geometry_buffer;
import polygon;
import outline;

export class Scene
{
public:
	Node* root;
	GeometryBuffer geom;
	

	Scene() {
		root = new Node();
	};

	~Scene() {
		for (int i = 0; i < root->children.size(); i++)
		{
			delete root->children.at(i);
		}
		delete root;
	};

	void add_node(glm::vec2 vertices[], int num_vertices, float outline_width,
		float red, float green, float blue,
		float ored, float ogreen, float oblue)
	{
		Node* node = new Node();
		for (int i = 0; i < num_vertices; i++)
		{
			node->add_vertex(vertices[i]);
		}
		node->outline_width = outline_width;
		node->color = glm::vec3(red, green, blue);
		node->outline_color = glm::vec3(ored, ogreen, oblue);

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
	}

	void render()
	{
		// eventually I will add recursive traversal and transformations
		geom.render();
	}
};