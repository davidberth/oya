module;

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

export module event;

export class Event {
public:
	virtual ~Event() = default;
};

export template<typename T>
class EventHandler {
public:
	using CallbackFn = std::function<void(const T&)>;
	std::vector<CallbackFn> callbacks;
};

class EventDispatcher {
private:
	std::unordered_map<std::type_index, void*> event_handlers;

public:
	EventDispatcher()
	{
		printf("EventDispatcher created\n");
	}
	template<typename T>
	void subscribe(std::function<void(const T&)> callback) {
		auto type_index = std::type_index(typeid(T));

		if (event_handlers.find(type_index) == event_handlers.end()) {
			event_handlers[type_index] = new EventHandler<T>();
		}

		auto handler = static_cast<EventHandler<T>*>(event_handlers[type_index]);
		handler->callbacks.push_back(callback);
	}

	template<typename T>
	void dispatch(const T& event) {
		auto type_index = std::type_index(typeid(T));

		if (event_handlers.size() == 0) {
			printf("No event handlers available!\n");
			return;
		}
		if (event_handlers.find(type_index) != event_handlers.end()) {
			auto handler = static_cast<EventHandler<T>*>(event_handlers[type_index]);
			for (auto& callback : handler->callbacks) {
				callback(event);
			}
		}
	}

	void clear() {
		event_handlers.clear();
	}

	~EventDispatcher() {
		for (auto& pair : event_handlers) {
			delete pair.second;
		}
	}
};

EventDispatcher* event_dispatcher;

export inline EventDispatcher* get_event_dispatcher() {
	return event_dispatcher;
}

export inline void init_event_dispatcher() {
	event_dispatcher = new EventDispatcher();
}

export inline void delete_event_dispatcher() {
	delete event_dispatcher;
}
