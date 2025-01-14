module;

#include <vector>

export module layer_stack;

import layer;

export class LayerStack
{
private:
	std::vector<Layer*> layers;
	unsigned int layer_insert_index = 0;

public:
	LayerStack() {}
	~LayerStack() {
		for (Layer* layer : layers)
		{
			delete layer;
		}

	}

	void add_layer(Layer* layer)
	{
		layers.emplace_back(layer);
	}

	std::vector<Layer*>::iterator begin() { return layers.begin(); }
	std::vector<Layer*>::iterator end() { return layers.end(); }
};

export inline LayerStack& get_layer_stack()
{
	static LayerStack layer_stack;
	return layer_stack;
}
