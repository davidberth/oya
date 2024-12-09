export module window_event;

import event;

export struct WindowEvent : public Event
{
    int width;
    int height;

    WindowEvent(int width, int height)
        : width(width), height(height) {}
};