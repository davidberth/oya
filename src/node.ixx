module;

#include <glm/glm.hpp>
#include <vector>

export module node;
import vertex;

export class Node
{
public:
	std::vector<glm::vec2> outline;
	std::vector<glm::vec2> buffer;
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	glm::vec3 color;
	glm::vec3 outline_color = { 1.0f, 1.0f, 1.0f };
	float outline_width;

	Node* parent;
	std::vector<Node*> children;

	Node() {
		vertices.clear();
		indices.clear();
	};
	~Node() {};

	void add_vertex(glm::vec2 pos)
	{
		outline.push_back(pos);
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