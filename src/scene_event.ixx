module;

export module scene_event;

import event;

export struct SceneEvent : public Event {
	int event_type;
	SceneEvent(int typ)
		: event_type(typ) {
	}
};