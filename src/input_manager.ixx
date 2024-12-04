module;

#include <unordered_map>
#include <GLFW/glfw3.h>
#include <loguru.hpp>

export module input_manager;

import event;
import key_event;
import input_event;
import event;


export class InputManager {
private:
    // map keyboard keys to game actions
    std::unordered_map<int, InputAction> key_bindings;

public:
    InputManager() {
        setup_default_bindings();
        event_dispatcher.subscribe<KeyEvent>([this](const KeyEvent& key_event) {
            on_key_event(key_event);
        });     
    }

    void on_key_event(const KeyEvent& key_event) {
        // check if we have a binding for this key
        auto it = key_bindings.find(key_event.key_code);
        if (it != key_bindings.end()) {
            // convert key event to input event
            bool is_pressed = (key_event.action == GLFW_PRESS); // assuming 1 is press
            auto input_event = InputEvent(it->second, is_pressed);
            LOG_F(INFO, "Input event: %d", static_cast<int>(input_event.action));
            // dispatch the input event
            event_dispatcher.dispatch(input_event);
        }
    }

    void bind_key(int key_code, InputAction action) {
        key_bindings[key_code] = action;
    }

private:
    void setup_default_bindings() {
        // default key bindings
        key_bindings[GLFW_KEY_W] = InputAction::up;    
        key_bindings[GLFW_KEY_S] = InputAction::down;  
        key_bindings[GLFW_KEY_A] = InputAction::left;  
        key_bindings[GLFW_KEY_D] = InputAction::right; 
        key_bindings[GLFW_KEY_1] = InputAction::one;
        key_bindings[GLFW_KEY_2] = InputAction::two;
        key_bindings[GLFW_KEY_3] = InputAction::three;  
    }
};

export InputManager input_manager;