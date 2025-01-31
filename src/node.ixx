module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

export module node;

import polygon;
import aabb;

export class Node
{
public:

	Node* parent;
	std::vector<Node*> children;

	Polygon polygon;

	// transform properties
	float rotate_delta = 0.0f;
	float angle = 0.0f;

	bool needs_transform = false;
	bool needs_child_transforms = false;
	int total_indices = 0;

	glm::mat4 local_transform;
	glm::mat4 world_transform;
	glm::vec2 centroid;

	AxisAlignedBoundingBox local_aabb = AxisAlignedBoundingBox();
	AxisAlignedBoundingBox total_aabb = AxisAlignedBoundingBox();

	Node() {

		local_transform = glm::mat4(1.0f);
		world_transform = glm::mat4(1.0f);
	};
	~Node() {};

	void set_transform(const glm::mat4& new_transform)
	{
		local_transform = new_transform;
	}

	glm::mat4 get_transform() const
	{
		return local_transform;
	}

	void setup_polygon()
	{
		polygon.setup();
		centroid = polygon.compute_centroid();
	}

	void compute_local_aabb()
	{
		local_aabb = AxisAlignedBoundingBox::compute_bounding_box(polygon);
	}

	void set_rotation(float angle)
	{
		local_transform = glm::translate(glm::mat4(1.0f), glm::vec3(centroid, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::translate(glm::mat4(1.0f), glm::vec3(-centroid, 0.0f));
	}

	void update(float dt)
	{
		angle += rotate_delta;
		if (angle > 6.283f) angle -= 2 * 6.283f;
		set_rotation(angle);
		for (Node* child : children)
		{
			child->update(dt);
		}
	}
};