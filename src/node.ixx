module;

#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

export module node;
import vertex;

export class Node
{
public:
	std::vector<glm::vec2> outline;
	std::vector<glm::vec2> buffer;
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int buffer_offset = 0;
	int index_offset = 0;
	
	glm::vec3 color;
	glm::vec3 outline_color = { 0.1f, 0.1f, 0.1f };
	float outline_width;

	Node* parent;
	std::vector<Node*> children;
	float rotate_delta = 0.0f;
	float angle=0.0f;

	bool needs_transform = false;
	bool needs_child_transforms = false;
	int total_indices = 0;


	glm::mat4 transform;
	glm::vec2 centroid;

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

	void compute_centroid()
	{
		centroid = glm::vec2(0.0f, 0.0f);
		for (int i = 0; i < outline.size(); i++)
		{
			centroid += outline.at(i);
		}
		centroid /= outline.size();
	}

	void set_transform(const glm::mat4& new_transform)
	{
		transform = new_transform;
	}

	glm::mat4 get_transform() const
	{
		return transform;
	}

	inline int get_buffer_offset()
	{
		return buffer_offset;
	}

	inline void set_buffer_offset(int offset)
	{
		buffer_offset = offset;
	}

	inline int get_index_offset()
	{
		return index_offset;
	}

	inline void set_index_offset(int offset)
	{
		index_offset = offset;
	}

	void set_rotation(float angle)
	{
		
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(centroid, 0.0f)) * 
			glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * 
			glm::translate(glm::mat4(1.0f), glm::vec3(-centroid, 0.0f));

	}

	void update(float dt)
	{
		// update logic here
		angle += rotate_delta;
		if (angle > 6.283f) angle -= 2 * 6.283f;
		set_rotation(angle);
		for (Node* child : children)
		{
			child->update(dt);
		}
	}
};