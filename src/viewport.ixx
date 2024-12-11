module;

#include <glm/glm.hpp>

export module viewport;

import event;
import window_event;



export struct Viewport {
	int width;
	int height;
public:
	Viewport()
	{
		width = 100;
		height = 100;
	}	

	void init() {
		event_dispatcher.subscribe<WindowEvent>([this](const WindowEvent& event) {
			width = event.width;
			height = event.height;
			});
	}

	glm::vec2 get_ndc_coords(double xpos, double ypos) {
		return glm::vec2(2.0f * xpos / width - 1.0f, 1.0f - 2.0f * ypos / height);
	}

	void set_size(int width, int height) {
		this->width = width;
		this->height = height;
	}
};

export Viewport viewport;