export module render;

import layer_stack;


export void render()
{
    // render
    for (auto layer : layer_stack)
    {
        if (layer->is_enabled())
        {
            layer->begin();
            layer->render();
            layer->end();
        }
    }

};