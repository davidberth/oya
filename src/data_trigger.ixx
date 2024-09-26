module;

#include <functional>
#include <vector>

export module data_trigger;

struct Callback
{
	std::function<void()> callback = nullptr;
	bool enabled = true;
};

export class DataTrigger
{
private: 
	bool triggered = false;
	std::unordered_map<int, Callback> listeners;
public:
	DataTrigger() {}
	~DataTrigger() {}
	virtual void log() {}
	void trigger() {
		triggered = true;
	}
	void reset() {
		triggered = false;
	}
	bool is_triggered() const {
		return triggered;
	}

	void add_listener(int id, std::function<void()> listener) {
		listeners[id].callback = listener;
	}

	void remove_listener(int id) {
		listeners.erase(id);
	}

	void enable_listener(int id) {
		listeners[id].enabled = true;
	}

	void disable_listener(int id) {
		listeners[id].enabled = false;
	}

	void update() {
		if (triggered) {
			notify_listeners();
		}
		reset();
	}
private:
	void notify_listeners() {
		for (const auto& [id, listener] : listeners) {
			if (listener.enabled) listener.callback();
		}
	}
};