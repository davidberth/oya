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

	const char* font_path = "c:\\Windows\\Fonts\\Arial.ttf";
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
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        LOG_F(INFO, "Initialized OpenGL");


        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        ImFont* font = io.Fonts->AddFontFromFileTTF(font_path, gui_data.font_size);
        IM_ASSERT(font != nullptr);

	}
	virtual void update() override
	{
	}

	virtual void begin() override
	{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::DockSpaceOverViewport();
        // ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

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

	void set_font_size(float size)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->ClearFonts();
        ImFont* font = io.Fonts->AddFontFromFileTTF(font_path, gui_data.font_size);
		IM_ASSERT(font != nullptr);
        io.Fonts->Build(); 
	}
};