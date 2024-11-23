module;

#include <glm/glm.hpp>
#include <vector>

export module node;
import vertex;

export class Node
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Node* parent;
	std::vector<Node*> children;


	Node() {
		vertices.clear();
		indices.clear();
	};
	~Node() {};

	void add_vertex(Vertex vertex)
	{
		vertices.push_back(vertex);
	}
	int get_num_vertices()
	{
		return vertices.size();
	}

	int get_num_indices()
	{
		return indices.size();
	}
};