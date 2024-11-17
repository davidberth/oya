module;

#include <vector>

export module shape_set;

import shape;

export class ShapeSet
{
private:
	std::vector<Shape> shapes;
	int buffer_size = 100;
public:
	ShapeSet() {};
	~ShapeSet() {};

	void add_shape(Shape s)
	{
		shapes.push_back(s);
	}

	int get_num_shapes()
	{
		return shapes.size();
	}

	int get_num_vertices()
	{
		int num_vertices = 0;
		for (auto s : shapes)
		{
			num_vertices += s.get_num_vertices();
		}
		return num_vertices;
	}
};