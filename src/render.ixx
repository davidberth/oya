export module render;

import layer_stack;


export void render()
{
    // render
    for (auto layer : layer_stack)
    {
        if (layer->do_render)
        {
            layer->begin();
            layer->render();
            layer->end();
        }
    
    }

};