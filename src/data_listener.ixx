module;

export module data_listener;

export class DataListener
{
private: 
	bool triggered = false;

public:
	DataListener() {}
	~DataListener() {}
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
};