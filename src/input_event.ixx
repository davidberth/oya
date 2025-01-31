export module input_event;

import event;

export enum class InputAction {
	up = 0,
	right,
	left,
	down,
	upright,
	upleft,
	downright,
	downleft,
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
	function_7,
	function_8,
	function_9,
	function_10,
	scroll_in,
	scroll_out,
	last
};

export struct InputEvent : public Event {
	InputAction action;
	bool is_pressed;

	InputEvent(InputAction a, bool pressed)
		: action(a), is_pressed(pressed) {
	}
};