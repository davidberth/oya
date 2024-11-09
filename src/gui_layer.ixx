module;

#include <string>
#include "loguru.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>

#include "GLFW/glfw3.h"

export module gui_layer;

import layer;
import gui_data;
import keyboard_data;
import mouse_data;
import viewport_data;
import gui_theme;
import camera;

export class GUILayer : public Layer
{
private:
    bool show_demo_window = true;
    bool show_another_window = false;


    const char* font_paths[3] = {"c:\\Windows\\Fonts\\Arial.ttf", 
                                "c:\\Windows\\Fonts\\times.ttf", 
                                "c:\\Windows\\Fonts\\pala.ttf"};

    unsigned int font_sizes[3] = { 24, 30, 38 };

    ImFont* font[3];

    int font_index = gui_data.font_index;
    
    
public:
	GUILayer(std::string pname) : Layer(pname) {};
	~GUILayer() {};
	virtual void init(GLFWwindow *window) override
	{
		__super::init(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
        io.ConfigFlags |= ImGuiWindowFlags_NoBackground;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        

        // Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        setup_gui_theme();

       
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        LOG_F(INFO, "Loading fonts");
		for (int i = 0; i < 3; ++i)
		{
			font[i] = io.Fonts->AddFontFromFileTTF(font_paths[i], font_sizes[i]);
			IM_ASSERT(font[i] != nullptr);
		}
        LOG_F(INFO, "done");
        
		add_listener(&mouse_scroll_data, &GUILayer::on_mouse_scroll);
		add_listener(&function_keyboard_data, &GUILayer::on_function_keyboard, true);
      
	

	}
	virtual void update() override
	{
	}

	virtual void begin() override
	{
		Layer::begin();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(font[font_index]);
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	}
	virtual void render() override
	{

         {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("World properties");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Background properties");               // Display some text (you can use a format strings too)
          
            ImGui::ColorEdit3("clear color", (float*)&gui_data.clear_color); // Edit 3 floats representing a color
            
			ImGui::Text("Window pos: x: %.3f, y: %.3f", mouse_pointer_data.xpos, mouse_pointer_data.ypos);
            
            // get the world coorindates at the mouse pointer
            
			glm::vec2 ndc_coords = viewport_data[0].win_to_ndc(glm::vec2(mouse_pointer_data.xpos, mouse_pointer_data.ypos));
            ImGui::Text("NDC pos  : x: %.3f, y: %.3f", ndc_coords.x, ndc_coords.y);
			glm::vec2 world_pos = camera.ndc_to_world(ndc_coords);
            
            ImGui::Text("World pos: x: %.3f, y: %.3f", world_pos.x, world_pos.y);




            ImGui::End();
        }


       
        ImGui::Begin("Viewport");
        // Get the available size within the viewport window
        ImVec2 viewport_size = ImGui::GetContentRegionAvail();

        // Display the texture in the ImGui window
        ImGui::Image((void*)(intptr_t)viewport_data[0].texture_index, viewport_size, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        viewport_data[0].set_size(int(viewport_size.x), int(viewport_size.y));

	}
	virtual void end()
	{

        ImGui::PopFont();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);

        Layer::end();

	}
	virtual void cleanup() override
	{
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        __super::cleanup();
	}

	void set_font_size(int index)
	{
        font_index = index;
        if (font_index < 0)
        {
            font_index = 0;
        }
        if (font_index > 2)
		{
			font_index = 2;
		}
	}

    // listeners

	void on_mouse_scroll()
	{
		LOG_F(INFO, "On Mouse Scroll Called in GUI layer now!");
		if (keyboard_data.left_control_down && (mouse_scroll_data.yoffset < 0.5 || mouse_scroll_data.yoffset > 0.5))
		{
			if (mouse_scroll_data.yoffset > 0.5)
			{
				set_font_size(font_index + 1);
			}
			else
			{
				set_font_size(font_index - 1);
			}
		}
	}

    // persistant
	void on_function_keyboard()
    {
		if (function_keyboard_data.F10_down)
		{
            disable();
		}
        else if (function_keyboard_data.F11_down)
		{
			enable();
		}
	}

};