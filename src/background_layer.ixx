module;

#include <string>
#include <glfw/glfw3.h>

export module background_layer;

import layer;
import gui_data;

export class BackgroundLayer : public Layer
{
    
public:
    BackgroundLayer(std::string pname) : Layer(pname) {};
    ~BackgroundLayer() {};
    virtual void init(GLFWwindow* window) override
    {
        Layer::init(window);

    }
    virtual void update() override
    {

    }
    virtual void begin()
    {
        Layer::begin();
    }
    virtual void render() override
    {
        glClearColor(gui_data.clear_color[0],
            gui_data.clear_color[1],
            gui_data.clear_color[2],
            gui_data.clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

   

    }
    virtual void end() override
    {
        Layer::end();
    }
    virtual void cleanup() override
    {
        Layer::cleanup();
    }


};