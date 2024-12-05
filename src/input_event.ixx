
export module input_event;

import event;

export enum class InputAction {
    up = 0,
    right,
    left,
    down,
    rotate_left,
    rotate_right,
    zoom_in,
    zoom_out,
    one,
    two,
    three,
    function_1,
    function_2,
    function_3,
    function_4,
    function_5,
    function_6,
    last
};

export struct InputEvent : public Event {
    InputAction action;
    bool is_pressed;

    InputEvent(InputAction a, bool pressed) 
        : action(a), is_pressed(pressed) {}
};