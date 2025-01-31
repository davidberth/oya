module;

#include <array>
#include <SDL3/SDL.h>
#include <unordered_map>

export module input_manager;

import event;
import input_event;
import mouse_event;
import key_event;


export class InputManager {
private:
	// map keyboard keys to game actions
	std::unordered_map<int, InputAction> key_bindings;
	std::array<bool, static_cast<size_t>(InputAction::last)> input_states;

public:
	float target_x;
	float target_y;

	InputManager() {
		setup_default_bindings();
		get_event_dispatcher()->subscribe<KeyEvent>([this](const KeyEvent& key_event) {
			on_key_event(key_event);
			});
		get_event_dispatcher()->subscribe<MouseMoveEvent>([this](const MouseMoveEvent& mouse_event) {
			target_x = mouse_event.xpos;
			target_y = mouse_event.ypos;
			});
		get_event_dispatcher()->subscribe<MouseScrollEvent>([this](const MouseScrollEvent& scroll_event) {
			if (scroll_event.yoffset > 0) {
				get_event_dispatcher()->dispatch(InputEvent(InputAction::scroll_in, true));
			}
			else {
				get_event_dispatcher()->dispatch(InputEvent(InputAction::scroll_out, true));
			}
			});

		for (int i = 0; i < static_cast<size_t>(InputAction::last); i++) {
			input_states[i] = false;
		}
	}

	void on_key_event(const KeyEvent& key_event) {
		// check if we have a binding for this key 
		auto it = key_bindings.find(key_event.key_code);
		if (it != key_bindings.end()) {
			// convert key event to input event
			bool is_pressed = (key_event.action);
			auto input_event = InputEvent(it->second, is_pressed);

			input_states[static_cast<size_t>(input_event.action)] = is_pressed;
			// dispatch the input event
			get_event_dispatcher()->dispatch(input_event);
		}
	}

	void bind_key(int key_code, InputAction action) {
		key_bindings[key_code] = action;
	}

	inline bool get_input_state(InputAction action) {
		return input_states[static_cast<size_t>(action)];
	}

private:
	void setup_default_bindings() {

		key_bindings[SDLK_UP] = InputAction::up;
		key_bindings[SDLK_DOWN] = InputAction::down;
		key_bindings[SDLK_LEFT] = InputAction::left;
		key_bindings[SDLK_RIGHT] = InputAction::right;
		key_bindings[SDLK_KP_8] = InputAction::up;
		key_bindings[SDLK_KP_2] = InputAction::down;
		key_bindings[SDLK_KP_4] = InputAction::left;
		key_bindings[SDLK_KP_6] = InputAction::right;
		key_bindings[SDLK_KP_7] = InputAction::upleft;
		key_bindings[SDLK_KP_9] = InputAction::upright;
		key_bindings[SDLK_KP_1] = InputAction::downleft;
		key_bindings[SDLK_KP_3] = InputAction::downright;
		key_bindings[SDLK_Q] = InputAction::rotate_left;
		key_bindings[SDLK_E] = InputAction::rotate_right;
		key_bindings[SDLK_R] = InputAction::zoom_in;
		key_bindings[SDLK_F] = InputAction::zoom_out;
		key_bindings[SDLK_1] = InputAction::one;
		key_bindings[SDLK_2] = InputAction::two;
		key_bindings[SDLK_3] = InputAction::three;
		key_bindings[SDLK_F1] = InputAction::function_1;
		key_bindings[SDLK_F2] = InputAction::function_2;
		key_bindings[SDLK_F3] = InputAction::function_3;
		key_bindings[SDLK_F4] = InputAction::function_4;
		key_bindings[SDLK_F5] = InputAction::function_5;
		key_bindings[SDLK_F6] = InputAction::function_6;
		key_bindings[SDLK_F7] = InputAction::function_7;
	}
};

InputManager* input_manager;

export inline InputManager* get_input_manager() {
	return input_manager;
}

export inline void init_input_manager() {
	input_manager = new InputManager();
}

export inline void delete_input_manager() {
	delete input_manager;
}
