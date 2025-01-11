module;

#include <string>
#include "GL/glew.h"


export module layer;


export class Layer
{
private:
	std::string name;

public:
	bool do_render = true;
	Layer(std::string pname) : name(pname) {}
	~Layer() {};
	virtual void init() {
	};
	
	virtual void update() {};
	virtual void begin() {
		
	};
	virtual void render() {};
	virtual void end() {
		
	};
	virtual void cleanup() {
	};	

	std::string get_name() { return name; };
};

