module;

#include <string>
#include "loguru.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"

export module gui_layer;

import layer;
import gui_data;
import keyboard_data;
import mouse_data;

export class GUILayer : public Layer
{
private:
    bool show_demo_window = true;
    bool show_another_window = false;


    const char* font_paths[3] = {"c:\\Windows\\Fonts\\Arial.ttf", 
                                "c:\\Windows\\Fonts\\times.ttf", 
                                "c:\\Windows\\Fonts\\pala.ttf"};

    unsigned int font_sizes[3] = { 16, 24, 32 };

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

        LOG_F(INFO, "Initialized OpenGL");


        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
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
        //ImGui::PopFont();
	

	}
	virtual void update() override
	{
	}

	virtual void begin() override
	{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(font[font_index]);
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	}
	virtual void render(int display_w, int display_h) override
	{
	
		 if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&gui_data.clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

	}
	virtual void end() override
	{

        ImGui::Begin("Viewport");

        // Get the available size within the viewport window
        ImVec2 viewport_size = ImGui::GetContentRegionAvail();

        // Retrieve the position of the viewport window
        ImVec2 viewport_pos = ImGui::GetWindowPos();
        // glViewport(viewport_pos.x, viewport_pos.y, (int)viewport_size.x, (int)viewport_size.y);

		gui_data.viewport_height = (int)viewport_size.y;
		gui_data.viewport_width = (int)viewport_size.x;

        ImGui::End();

        ImGui::PopFont();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);

      

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

};