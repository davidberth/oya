export;

#include <glm/glm.hpp>
#include <vector>

export module shape;


export class Shape
{
private:
	std::vector<glm::vec2> position;
	std::vector<glm::vec3> color;
	std::vector<unsigned int> indices;

public:
	Shape() {};
	~Shape() {};

	void add_vertex(glm::vec2 pos, glm::vec3 col)
	{
		position.push_back(pos);
		color.push_back(col);
	}

	void add_index(unsigned int i)
	{
		indices.push_back(i);
	}

	void add_vertex(float x, float y, float r, float g, float b)
	{
		position.push_back(glm::vec2(x, y));
		color.push_back(glm::vec3(r, g, b));
	}

	int get_num_vertices()
	{
		return position.size();
	}

	int get_num_indices()
	{
		return indices.size();
	}
};