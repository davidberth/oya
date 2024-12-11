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
	glm::vec3 outline_color = { 0.1f, 0.1f, 0.1f };
	float outline_width;

	Node* parent;
	std::vector<Node*> children;

	glm::mat4 transform;

	Node() {
		vertices.clear();
		indices.clear();
		transform = glm::mat4(1.0f);
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

	void set_transform(const glm::mat4& new_transform)
	{
		transform = new_transform;
	}

	glm::mat4 get_transform() const
	{
		return transform;
	}
};