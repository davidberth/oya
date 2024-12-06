export module key_event;

import event;

export struct KeyEvent : public Event {
    int key_code;
    int action;
    int mods;
    KeyEvent(int code, int action) : key_code(code), action(action), mods(mods) {}
};

