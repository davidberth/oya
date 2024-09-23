module;

#include <vector>

export module viewport_data;

import data_trigger;

export class ViewportData : public DataTrigger
{
	int size_x = 0;
	int size_y = 0;
	int pos_x = 0;
	int pos_y = 0;
};

std::vector<ViewportData> viewports_data;