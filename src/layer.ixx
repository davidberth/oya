module;

#include <string>
#include "loguru.hpp"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

export module layer;


export class Layer
{
private:
	std::string name;
	bool enabled = true;

public:
	Layer(std::string pname) : name(pname) {}
	~Layer() {};
	virtual void init(GLFWwindow *) {
		LOG_F(INFO, "Layer %s initialized", name.c_str());
	};
	
	virtual void update() {};
	virtual void begin() {
		
	};
	virtual void render() {};
	virtual void end() {
		
	};
	virtual void cleanup() {
		LOG_F(INFO, "Layer %s cleaned up", name.c_str());
		
	};	

	std::string get_name() { return name; };
};

