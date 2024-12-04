export module input_event;

import event;

export enum class InputAction {
    up,
    right,
    left,
    down,
    one,
    two,
    three,
    mod1,
    mod2,
};

export struct InputEvent : public Event {
    InputAction action;
    bool is_pressed;

    InputEvent(InputAction a, bool pressed) 
        : action(a), is_pressed(pressed) {}
};