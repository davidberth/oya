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
import gui_theme;
import camera;
import persistent_data;
import event;
import input_event;
import viewport;
import input_manager;

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

        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
       
        get_imgui_style();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        LOG_F(INFO, "Loading fonts");
		for (int i = 0; i < 3; ++i)
		{
			font[i] = io.Fonts->AddFontFromFileTTF(font_paths[i], font_sizes[i]);
			IM_ASSERT(font[i] != nullptr);
		}
        LOG_F(INFO, "done");

		event_dispatcher.subscribe<InputEvent>([this](const InputEvent& event) { on_input(event); });
   
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

        float window_x = input_manager.target_x;
        float window_y = input_manager.target_y;
        glm::vec2 ndc_coords = viewport.get_ndc_coords(window_x, window_y);
        glm::vec2 world_pos = camera.ndc_to_world_at_z(ndc_coords, 0.0f);

        ImGuiIO& io = ImGui::GetIO();
        float framerate = io.Framerate;

        ImGui::Begin("Properties");  
        ImGui::Text("Performance");
        ImGui::Text(" Framerate: %.1f FPS", framerate);
        ImGui::Text("Background properties");              
        //ImGui::ColorEdit3(" clear color", (float*)&gui_data.clear_color); 
        ImGui::Separator();
		ImGui::Text("Mouse properties");
        ImGui::Text(" Window pos: x: %.0f, y: %.0f", window_x, window_y);
		ImGui::Text(" NDC pos: x: %.3f, y: %.3f", ndc_coords.x, ndc_coords.y);
        ImGui::Text(" World pos: x: %.3f, y: %.3f", world_pos.x, world_pos.y);
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



    void on_input(const InputEvent& event) {
		LOG_F(INFO, "Key Input: %d %d ", event.action);

        if (event.action == InputAction::one)
        {
           persistent_data.font_size = 0;
           set_font_size(0);
        }
        else if (event.action == InputAction::two)
        {
            persistent_data.font_size = 1;
            set_font_size(1);
        }
        else if (event.action == InputAction::three)
        {
            persistent_data.font_size = 2;
            set_font_size(2);
        }
	}

};

