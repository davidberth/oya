module;

#include "loguru.hpp"

export module keyboard_data;

import data_trigger;

export struct KeyboardData : public DataTrigger {
	bool left_control_down = false;
	bool left_shift_down = false;

	bool left_down = false;
	bool right_down = false;
	bool up_down = false;
	bool down_down = false;
	bool rotate_left_down = false;
	bool rotate_right_down = false;
	bool zoom_out_down = false;
	bool zoom_in_down = false;

	void log()
	{
		LOG_F(INFO, "KeyboardData: left_control_down: %d, left_shift_down: %d", left_control_down, left_shift_down);
		LOG_F(INFO, "KeyboardData: left_down: %d, right_down: %d, up_down: %d, down_down: %d", left_down, right_down, up_down, down_down);
	}

};

export struct FunctionKeyboardData : public DataTrigger {
	bool F10_down = false;
	bool F11_down = false;

	void log()
	{
		LOG_F(INFO, "FunctionKeyboardData: F10 down: %d, F11 down: %d", F10_down, F11_down);
	}

};

export KeyboardData keyboard_data;
export FunctionKeyboardData function_keyboard_data;