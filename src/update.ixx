export module update;

import layer_stack;
import updatable_manager;


export void update()
{

	// update updatables
    get_updatable_manager().update_all();

	// update layers
    for (auto layer : get_layer_stack())
    {
  
        layer->update();
    
    }

};