export module gui_data;

import data_trigger;

export struct GUIData : public DataTrigger {
	float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	int viewport_width = 800;
	int viewport_height = 600;
};

export GUIData gui_data;
