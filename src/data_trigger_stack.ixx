module;

#include <vector>
#include "loguru.hpp"

export module data_trigger_stack;

import data_trigger;

export class DataTriggerStack
{
private:
	std::vector<DataTrigger*> data_triggers;
	

public:
	DataTriggerStack() {}
	~DataTriggerStack() {}

	void add_data_trigger(DataTrigger* trigger)
	{
		LOG_F(INFO, "Data Trigger added");
		data_triggers.emplace_back(trigger);
	}

	std::vector<DataTrigger*>::iterator begin() { return data_triggers.begin(); }
	std::vector<DataTrigger*>::iterator end() { return data_triggers.end(); }
};