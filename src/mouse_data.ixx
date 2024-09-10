module;

#include "loguru.hpp"

export module mouse_data;

export struct MouseData {
	float xpos = 0;
	float ypos = 0;

	int mouse_button = 0;
	double scroll = 0;


	void log()
	{
		LOG_F(INFO, "MouseData: xpos: %f, ypos: %f, button: %d, scroll: %f", xpos, ypos, mouse_button, scroll);
	}
};

export MouseData mouse_data;