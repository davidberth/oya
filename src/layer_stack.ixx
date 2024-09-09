module; 

#include <vector>
#include "loguru.hpp"

export module layer_stack;

import layer;

export class LayerStack
{
private:
	std::vector<Layer*> layers;
	unsigned int layer_insert_index = 0;

public:
	LayerStack() {}
	~LayerStack() {}

	void add_layer(Layer* layer)
	{
		LOG_F(INFO, "Layer %s added", layer->get_name().c_str());
		layers.emplace_back(layer);
	}

	void cleanup()
	{
		LOG_F(INFO, "Cleaning up layer stack");
		for (Layer* layer : layers)
		{
			layer->cleanup();
			delete layer;
		}
	}

	std::vector<Layer*>::iterator begin() { return layers.begin(); }
	std::vector<Layer*>::iterator end() { return layers.end(); }
};