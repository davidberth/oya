module;

#include <string>
#include "loguru.hpp"
#include <GLFW/glfw3.h>

export module layer;

export class Layer
{
private:
	std::string name;
public:
	Layer(std::string pname) : name(pname) {};
	~Layer() {};
	virtual void init(GLFWwindow *) {
		LOG_F(INFO, "Layer %s initialized", name.c_str());
	};
	virtual void update() {};
	virtual void begin() {};
	virtual void render(int, int) {};
	virtual void end() {};
	virtual void cleanup() {
		LOG_F(INFO, "Layer %s cleaned up", name.c_str());
	};

	std::string get_name() { return name; };
};