module;

#include "loguru.hpp"

export module keyboard_data;

export struct KeyboardData {
	bool left_control_down = false;
	bool left_shift_down = false;

	void log()
	{
		LOG_F(INFO, "KeyboardData: left_control_down: %d, left_shift_down: %d", left_control_down, left_shift_down);
	}

};

export KeyboardData keyboard_data;