export module mouse_event;

import event;

export enum MouseButton {
	left = 0,
	right = 1,
	middle = 2
};

export enum MouseAction {
	press = 0,
	release = 1
};

export struct MouseMoveEvent : public Event {
    double xpos;
    double ypos;

    MouseMoveEvent(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}
};

export struct MouseButtonEvent : public Event {
    MouseButton button;
    MouseAction action;


    MouseButtonEvent(MouseButton button, MouseAction action) : button(button), action(action) {}
};

export struct MouseScrollEvent : public Event {
    double xoffset;
    double yoffset;

    MouseScrollEvent(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
};