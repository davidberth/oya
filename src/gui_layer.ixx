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
import persistent_data;
import window_data;

export class GUILayer : public Layer
{
private:
    bool show_demo_window = true;
    bool show_another_window = false;


    const char* font_paths[3] = {"c:\\Windows\\Fonts\\Arial.ttf", 
                                "c:\\Windows\\Fonts\\times.ttf", 
                                "c:\\Windows\\Fonts\\pala.ttf"};

    unsigned int font_sizes[3] = { 20, 30, 38 };

    ImFont* font[3];
   
    
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

        ImGui::PushFont(font[persistent_data.font_size]);
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	}
	virtual void render() override
	{


        ImGui::Begin("Viewport");
        // Get the available size within the viewport window
		ImVec2 window_pos = ImGui::GetWindowPos();
        ImVec2 viewport_pos = ImGui::GetWindowContentRegionMin();
        ImVec2 viewport_size = ImGui::GetContentRegionAvail();

        ImVec2 mouse_pos_screen = ImGui::GetMousePos();
        ImVec2 mouse_pos_viewport = ImVec2(mouse_pos_screen.x - (viewport_pos.x + window_pos.x),
            mouse_pos_screen.y - (viewport_pos.y + window_pos.y));

        // Display the texture in the ImGui window
        ImGui::Image((void*)(intptr_t)viewport_data.texture_index, viewport_size, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        viewport_data.set_size(int(viewport_size.x), int(viewport_size.y));
        glm::vec2 ndc_coords = viewport_data.win_to_ndc(glm::vec2(mouse_pos_viewport.x, mouse_pos_viewport.y));
        glm::vec2 world_pos = camera.ndc_to_world_at_z(ndc_coords, 0.0f);

        ImGuiIO& io = ImGui::GetIO();
        float framerate = io.Framerate;

        ImGui::Begin("Properties");  
        ImGui::Text("Performance");
        ImGui::Text(" Framerate: %.1f FPS", framerate);
        ImGui::Text("Background properties");              
        ImGui::ColorEdit3(" clear color", (float*)&gui_data.clear_color); 
        ImGui::Separator();
		ImGui::Text("Mouse properties");
        ImGui::Text(" Window pos: x: %.3f, y: %.3f", mouse_pointer_data.xpos, mouse_pointer_data.ypos);
		ImGui::Text(" NDC pos: x: %.3f, y: %.3f", ndc_coords.x, ndc_coords.y);
        ImGui::Text(" World pos: x: %.3f, y: %.3f", world_pos.x, world_pos.y);
        ImGui::Separator();
		ImGui::Text("Viewport properties");
		ImGui::Text(" Viewport pos: x: %.3f, y: %.3f", viewport_pos.x, viewport_pos.y);
		ImGui::Text(" Viewport size: x: %.3f, y: %.3f", viewport_size.x, viewport_size.y);
		ImGui::Separator();
		ImGui::Text("Camera properties");
		ImGui::Text(" Camera pos: x: %.3f, y: %.3f", camera.position.x, camera.position.y);
		ImGui::Text(" Camera height: %.3f", camera.height);


        ImGui::End();

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
        int font_index = index;
        if (font_index < 0)
        {
            font_index = 0;
        }
        if (font_index > 2)
		{
			font_index = 2;
		}
		persistent_data.font_size = font_index; 
	}

    // listeners

	void on_mouse_scroll()
	{
		LOG_F(INFO, "On Mouse Scroll Called in GUI layer now!");
		if (keyboard_data.left_control_down && (mouse_scroll_data.yoffset < 0.5 || mouse_scroll_data.yoffset > 0.5))
		{
			if (mouse_scroll_data.yoffset > 0.5)
			{
				set_font_size(persistent_data.font_size + 1);
			}
			else
			{
				set_font_size(persistent_data.font_size - 1);
			}
		}
	}

    // persistant
	void on_function_keyboard()
    {
		if (function_keyboard_data.F10_down)
		{
            disable();
			viewport_data.active = false;
            viewport_data.height = window_data.height;
			viewport_data.width = window_data.width;
		}
        else if (function_keyboard_data.F11_down)
		{
			enable();
			viewport_data.active = true;
		}
	}

};

