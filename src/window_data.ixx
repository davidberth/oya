export module window_data;

import data_trigger;

export struct WindowData : public DataTrigger {
	int width;
	int height;
};

export WindowData window_data;