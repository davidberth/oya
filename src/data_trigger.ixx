module;

#include <functional>
#include <vector>

export module data_trigger;

export class DataTrigger
{
private: 
	bool triggered = false;
	std::unordered_map<int, std::function<void()>> listeners;
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
		listeners[id] = listener;
	}

	void remove_listener(int id) {
		listeners.erase(id);
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
			listener();
		}
	}
};