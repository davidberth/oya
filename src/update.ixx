export module update;

import layer_stack;
import updatable_manager;
import data_trigger_stack;
import data_trigger;

export void update()
{

    // update data triggers
    for (auto data_trigger : data_trigger_stack)
    {
        data_trigger->update();
    }

	// update updatables
    updatable_manager.update_all();

	// update layers
    for (auto layer : layer_stack)
    {
        if (layer->is_enabled())
        {
            layer->update();
        }
    }

};