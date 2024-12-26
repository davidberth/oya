module;

#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

export module node;

import polygon;

export class Node
{
public:
	
	Node* parent;
	std::vector<Node*> children;

	Polygon polygon;

	// transform properties
	float rotate_delta = 0.0f;
	float angle=0.0f;

	bool needs_transform = false;
	bool needs_child_transforms = false;
	int total_indices = 0;

	glm::mat4 transform;
	glm::vec2 centroid;

	Node() {

		transform = glm::mat4(1.0f);
	};
	~Node() {};

	void set_transform(const glm::mat4& new_transform)
	{
		transform = new_transform;
	}

	glm::mat4 get_transform() const
	{
		return transform;
	}

	void setup_polygon()
	{
		polygon.setup();
		centroid = polygon.compute_centroid();
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