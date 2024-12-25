module;

#include <string>

export module scene_event;

import event;

export enum class SceneEventType {
    load = 0,
    save,
};

export struct SceneEvent : public Event {
    SceneEventType event_type;
    std::string scene_path;

    SceneEvent(SceneEventType typ, std::string scene_pth)
        : event_type(typ), scene_path(scene_pth) {
    }
};