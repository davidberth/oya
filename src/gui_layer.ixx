module;

#include <string>

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
import render_stats_event;
import scene_event;

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
	int num_draw_calls = 0;
    float average_indices = 0.0f;
	float average_triangles = 0.0f;
	int num_draw_calls_debug = 0;
	float average_indices_debug = 0.0f;
	float average_outlines_debug = 0.0f;
   
    
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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
       
        get_imgui_style();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

   
		for (int i = 0; i < 3; ++i)
		{
			font[i] = io.Fonts->AddFontFromFileTTF(font_paths[i], font_sizes[i]);
			IM_ASSERT(font[i] != nullptr);
		}
   
		get_event_dispatcher().subscribe<InputEvent>([this](const InputEvent& event) { on_input(event); });
        get_event_dispatcher().subscribe<RenderStatsEvent>([this](const RenderStatsEvent& event) {
            if (event.render_set == RenderStatsEventSet::world_geometry)
            {
                num_draw_calls = event.num_draw_calls;
                average_indices = event.average_indices;
                average_triangles = event.average_elements;
            }
			else if (event.render_set == RenderStatsEventSet::debug_render)
            {
				num_draw_calls_debug = event.num_draw_calls;
				average_indices_debug = event.average_indices;
				average_outlines_debug = event.average_elements;
            }
            });
            
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

        ImGui::PushFont(font[get_persistent_data().font_size]);
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	}
	virtual void render() override
	{
        float window_x = get_input_manager().target_x;
        float window_y = get_input_manager().target_y;
        glm::vec2 ndc_coords = get_viewport().get_ndc_coords(window_x, window_y);
        glm::vec2 world_pos = get_camera().ndc_to_world_at_z(ndc_coords, 0.0f);

        ImGuiIO& io = ImGui::GetIO();
        float framerate = io.Framerate;

        // Properties window
        ImGui::Begin("Properties");  
        ImGui::Text("Performance");
        ImGui::Text(" Framerate: %.1f FPS", framerate);
        ImGui::Text("Background properties");              
        ImGui::Separator();
		ImGui::Text("Mouse properties");
        ImGui::Text(" Window pos: x: %.0f, y: %.0f", window_x, window_y);
		ImGui::Text(" NDC pos: x: %.3f, y: %.3f", ndc_coords.x, ndc_coords.y);
        ImGui::Text(" World pos: x: %.3f, y: %.3f", world_pos.x, world_pos.y);
        ImGui::Separator();
		ImGui::Text("Camera properties");
		ImGui::Text(" Camera pos: x: %.3f, y: %.3f", get_camera().position.x, get_camera().position.y);
		ImGui::Text(" Camera height: %.3f", get_camera().height);
        ImGui::Separator();
        ImGui::Text("Render properties");
        ImGui::Text("Draw calls: %d", num_draw_calls);
		ImGui::Text("Average indices: %.1f", average_indices);
		ImGui::Text("Average triangles: %.1f", average_triangles);
		ImGui::Text("Total triangles: %d", int(num_draw_calls * average_triangles));
		ImGui::Separator();
        ImGui::Text("Debug Render properties");
        ImGui::Text("Draw calls: %d", num_draw_calls_debug);
        ImGui::Text("Average indices: %.1f", average_indices_debug);
        ImGui::Text("Average outlines: %.1f", average_outlines_debug);
        ImGui::Text("Total outlines: %d", int(num_draw_calls_debug * average_outlines_debug));
        ImGui::End();


        // Scenes window
        ImGui::Begin("Scenes");
        ImGui::Columns(3, "scenes_columns", false);
        ImGui::SetColumnWidth(0, 100.0f); // scene name column
        ImGui::SetColumnWidth(1, 60.0f);  // load button column
        ImGui::SetColumnWidth(2, 60.0f);  // save button column

        const int num_scenes = 5; // adjust number of scenes as needed
        for (int scene_index = 0; scene_index < num_scenes; scene_index++)
        {
            // Column 1: Scene name
            ImGui::Text("Scene %d", scene_index + 1);
            ImGui::NextColumn();

            // Column 2: Load button
            if (ImGui::Button(("Load##" + std::to_string(scene_index)).c_str()))
            {
				std::string scene_path = "scenes/scene" + std::to_string(scene_index + 1) + ".scn";
                get_event_dispatcher().dispatch(SceneEvent(SceneEventType::load, scene_path));
            }
            ImGui::NextColumn();

            // Column 3: Save button
            if (ImGui::Button(("Save##" + std::to_string(scene_index)).c_str()))
            {
                std::string scene_path = "scenes/scene" + std::to_string(scene_index + 1) + ".scn";
				get_event_dispatcher().dispatch(SceneEvent(SceneEventType::save, scene_path));
            }
            ImGui::NextColumn();
        }

        ImGui::Columns(1); // reset back to 1 column layout
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
		get_persistent_data().font_size = font_index;
	}

    void on_input(const InputEvent& event) {
		
        if (event.is_pressed)
        {

            if (event.action == InputAction::one)
            {
            get_persistent_data().font_size = 0;
            set_font_size(0);
            }
            else if (event.action == InputAction::two)
            {
                get_persistent_data().font_size = 1;
                set_font_size(1);
            }
            else if (event.action == InputAction::three)
            {
                get_persistent_data().font_size = 2;
                set_font_size(2);
            }
            if (event.action == InputAction::function_7)
            {
                this->do_render = !this->do_render;
            }
        }
	}
};

