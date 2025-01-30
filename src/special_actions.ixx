module;

#include <string>

export module special_actions;

import event;
import input_event;
import scene_event;

struct size_option
{
	int index;
	InputAction key;
	float size_ratio_x;
	float size_ratio_y;
	int monitor;
};

size_option size_options[6] =
{ {0, InputAction::function_1, 0.2f, 0.3f, 0},
{1, InputAction::function_2, 0.4f, 0.5f, 0},
{2, InputAction::function_3, 0.65f, 0.75f, 0},
{3, InputAction::function_4, 0.8f, 0.85f, 0},
{4, InputAction::function_5, 1.0f, 1.0f, 0},
{5, InputAction::function_6, 1.0f, 1.0f, 1},
};


class SpecialActions {

public:
	SpecialActions()
	{
		printf("SpecialActions created\n");
		get_event_dispatcher()->subscribe<InputEvent>([this](const InputEvent& event) { on_input(event); });

	}
	void on_input(const InputEvent& event) {

		if (event.is_pressed)
		{
			if (event.action == InputAction::function_1)
			{
				get_event_dispatcher()->dispatch(SceneEvent(0));
			}
		}


		/*
		if (event.is_pressed)
		{
			for (int i = 0; i < 6; i++)
			{
				if (event.action == size_options[i].key)
				{
					set_window_size(size_options[i].size_ratio_x, size_options[i].size_ratio_y, size_options[i].monitor);
				}
			}
		}
		*/
	}
};

SpecialActions* special_actions;

export inline SpecialActions* get_special_actions() {
	return special_actions;
}

export inline void init_special_actions() {
	special_actions = new SpecialActions();
}

export inline void delete_special_actions() {
	delete special_actions;
}
