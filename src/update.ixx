export module update;

import layer_stack;
import updatable_manager;


export void update()
{

	// update updatables
    updatable_manager.update_all();

	// update layers
    for (auto layer : layer_stack)
    {
  
        layer->update();
    
    }

};