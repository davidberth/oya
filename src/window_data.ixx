module;

#include <glm/glm.hpp>

export module window_data;

import data_trigger;

export struct WindowData : public DataTrigger {
	int width;
	int height;

	glm::vec2 get_ndc_coords(double xpos, double ypos) {
		return glm::vec2(2.0f * xpos / width - 1.0f, 1.0f - 2.0f * ypos / height);
	}
};

export WindowData window_data;