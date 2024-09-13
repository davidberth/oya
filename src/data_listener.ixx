module;

#include <functional>
#include <vector>

export module data_listener;

export class DataListener
{
private: 
	bool triggered = false;
	std::vector<std::function<void()>> listeners;
public:
	DataListener() {}
	~DataListener() {}
	virtual void log() {}
	
	void trigger() {
		triggered = true;
		notify_listeners();
	}
	void reset() {
		triggered = false;
	}
	bool is_triggered() const {
		return triggered;
	}

	void add_listener(std::function<void()> listener) {
		listeners.push_back(listener);
	}
private:
	void notify_listeners() {
		for (const auto& listener : listeners)
		{
			listener();
		}
		reset();
	}
};