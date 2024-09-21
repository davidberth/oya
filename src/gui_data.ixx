export module gui_data;

import data_listener;

export struct GUIData : public DataListener {
	float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	int font_index = 1;
	int viewport_width = 800;
	int viewport_height = 600;
};

export GUIData gui_data;
