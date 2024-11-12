module;


#include <iostream>
#include <glm/glm.hpp>

export module viewport_data;

import data_trigger;

export class ViewportData : public DataTrigger
{
public:
	int width = 100;
	int height = 100;
	int old_width = 100;
	int old_height = 100;

	int texture_index = 0;
	bool active = true;

	glm::vec2 win_to_ndc(glm::vec2 win_pos) const
	{
		float ndc_x = (2.0f * win_pos.x) / width - 1.0f;
		float ndc_y = 1.0f - (2.0f * win_pos.y) / height;

		return glm::vec2(ndc_x, ndc_y);

	}

	void set_size(int w, int h)
	{
		width = w;
		height = h;
		if (width != old_width || height != old_height)
		{
			trigger();
			old_width = width;
			old_height = height;
		}
	}
};

export ViewportData viewport_data;
