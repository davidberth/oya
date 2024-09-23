module;

#include "loguru.hpp"

export module mouse_data;

import data_trigger;

export struct MousePointerData : public DataTrigger {
	float xpos = 0;
	float ypos = 0;

	void log()
	{
		LOG_F(INFO, "MouseData: xpos: %f, ypos: %f", xpos, ypos);
	}
};

export struct MouseButtonData : public DataTrigger {
	int button = 0;
	int action = 0;
	
	void log()
	{
		LOG_F(INFO, "MouseButtonData: button: %d, action: %d", button);
	}
};

export struct MouseScrollData : public DataTrigger {
	float yoffset = 0;

	void log()
	{
		LOG_F(INFO, "MouseScrollData: yoffset: %f", yoffset);
	}
};


export MousePointerData mouse_pointer_data;
export MouseButtonData mouse_button_data;
export MouseScrollData mouse_scroll_data;
