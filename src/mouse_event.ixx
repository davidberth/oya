export module mouse_event;

import event;

export struct MouseMoveEvent : public Event {
    double xpos;
    double ypos;

    MouseMoveEvent(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}
};

export struct MouseButtonEvent : public Event {
    int button;
    int action;
    int mods;

    MouseButtonEvent(int button, int action, int mods) : button(button), action(action), mods(mods) {}
};

export struct MouseScrollEvent : public Event {
    double xoffset;
    double yoffset;

    MouseScrollEvent(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
};