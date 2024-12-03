export module key_event;

import event;

export namespace keycodes {
    constexpr int F10 = 299;
    constexpr int F11 = 300;
}

export struct KeyEvent : public Event {
    int key_code;
    int action;
    KeyEvent(int code, int action) : key_code(code), action(action) {}
};

