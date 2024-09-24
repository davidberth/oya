module;

#include <vector>

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

export ViewportData viewport_data[1];
