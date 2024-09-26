module;

#include <functional>

export module listener;

import data_trigger;

export class Listener
{
private:
	static int next_id;
	int id;

	bool listening = true;

	std::vector<DataTrigger*> triggers;

public:

	Listener() : id(next_id++) {}

	int get_id() const {
		return id;
	}

	template <typename T>
	void add_listener(DataTrigger* trigger, void (T::* func)(), bool persist = false) {
		auto bound_func = std::bind(func, static_cast<T*>(this));
		trigger->add_listener(get_id(), bound_func);

		if (!persist) triggers.push_back(trigger);
	}


	template <typename T>
	void remove_listener(DataTrigger* trigger) {
		trigger->remove_listener(get_id());

		triggers.erase(std::remove(triggers.begin(), triggers.end(), trigger), triggers.end());
	}

	void listen() {
		listening = true;

		for (auto trigger : triggers) {
			trigger->enable_listener(get_id());
		}

	}
	void unlisten() {
		listening = false;

		for (auto trigger : triggers) {
			trigger->disable_listener(get_id());
		}
	}
};

int Listener::next_id = 0;