module;

export module data_listener;

export class DataListener
{
private: 
	bool change_flagged = false;
	const char* name;

public:
	DataListener(const char *pname)
	{
		name = pname;
	}
	virtual void log() {}
	
	void changed() {
		change_flagged = true;
	}
	void reset() {
		change_flagged = false;
	}
	bool is_changed() {
		return change_flagged;
	}
};