module;

#include <glm/glm.hpp>
#include <vector>

export module node;
import vertex;

export class Node
{
private:
	Vertex* vertices = NULL;
	unsigned int *indices = NULL;

	unsigned num_vertices = 0;
	unsigned num_indices = 0;

public:
	Node(Vertex* start_vertex, unsigned int *start_index, int num_verticesl, int num_indicesl) {
		vertices = start_vertex;
		indices = start_index;
		num_vertices = num_verticesl;
		num_indices = num_indicesl;
	};
	~Node() {};

	
	int get_num_vertices()
	{
		return num_vertices;
	}

	int get_num_indices()
	{
		return num_indices;
	}
};