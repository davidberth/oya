module;

export module special_actions;

import event;
import input_event;
import window;

class SpecialActions {

public:
    SpecialActions()
    {
        event_dispatcher.subscribe<InputEvent>([this](const InputEvent& event) { on_input(event); });
      
    }
    void on_input(InputEvent& event) {
        if (event.is_pressed)
        {
            if (event.action == InputAction::function_1)
            {
                // window set size to 1
            }
            else if (event.action == InputAction::function_2)
            {
                // do something
            }
            else if (event.action == InputAction::function_3)
            {
                // do something
            }



        }
    }
};