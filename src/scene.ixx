module;

export module scene;

import node;
import geometry_buffer;

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

	void add_node(Vertex vertices[], unsigned int indices[], int num_vertices, int num_indices)
	{
		Node* node = new Node();
		for (int i = 0; i < num_vertices; i++)
		{
			node->add_vertex(vertices[i]);
		}
		for (int i = 0; i < num_indices; i++)
		{
			node->indices.push_back(indices[i]);
		}
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