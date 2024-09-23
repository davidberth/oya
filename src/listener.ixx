module;

#include <functional>

export module listener;

import data_trigger;

export class Listener
{
private:
	static int next_id;
	int id;

public:

	Listener() : id(next_id++) {}

	int get_id() const {
		return id;
	}

	template <typename T>
	void add_listener(DataTrigger* trigger, void (T::* func)()) {
		auto bound_func = std::bind(func, static_cast<T*>(this));
		trigger->add_listener(get_id(), bound_func);
	}

	template <typename T>
	void remove_listener(DataTrigger* trigger) {
		trigger->remove_listener(get_id());
	}
};

int Listener::next_id = 0;