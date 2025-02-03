module;

#include <fstream>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <sstream>
#include <string>
#include <vector>
export module font_manager;

import textured_quad;
import render_set;
import shader;
import vertex;
import sdl_data;

export class FontManager
{
public:
	float tex_coord[128][4];
	int horizontal_spacing_pixel = 2;
	int vertical_spacing_pixel = 4;

	std::string font_name = "";

	SDL_Surface* font_surface = nullptr;
	RenderSet<TextureVertex>* render_set = nullptr;

	TexturedQuad font_quad;
	glm::mat4x4 identity = glm::mat4(1.0f);


	FontManager() {
		memset(tex_coord, 0, sizeof(tex_coord));
	};
	~FontManager() {
	};

	void init()
	{
		SDL_GPUShader* vertex_shader_font = load_shader(sdl_device, "font_vert", 0, 0, 0, 0);
		SDL_GPUShader* fragment_shader_font = load_shader(sdl_device, "font_frag", 1, 0, 0, 0);

		font_quad.add_vertex(glm::vec2(0.2f, 0.2f));
		font_quad.add_vertex(glm::vec2(0.4f, 0.4f));

		font_quad.setup();

		render_set = new RenderSet<TextureVertex>();
		render_set->init(RenderGeometryType::TRIANGLE_LIST, 50, vertex_shader_font, fragment_shader_font, 1000, 1000,
			font_surface);
	}

	void update()
	{
		render_set->begin();
		render_set->add(&font_quad, &identity);
		render_set->end();
	}

	void load_font_layout(const std::string& layout_path) {
		std::ifstream file(layout_path);
		if (!file.is_open()) {
			SDL_Log("Could not open font layout file!");
			return;
		}

		std::string line;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string item;
			std::vector<std::string> tokens;
			while (std::getline(ss, item, ',')) {
				tokens.push_back(item);
			}

			if (tokens.size() >= 10) {
				int index = std::stoi(tokens[0]);
				tex_coord[index][0] = std::stof(tokens[6]);
				tex_coord[index][1] = std::stof(tokens[7]);
				tex_coord[index][2] = std::stof(tokens[8]);
				tex_coord[index][3] = std::stof(tokens[9]);
			}
		}
	}

	void load_font(const std::string& font_name) {
		this->font_name = font_name;
		std::string font_path = "resources/textures/" + font_name + ".bmp";
		font_surface = SDL_LoadBMP(font_path.c_str());
		if (font_surface == NULL) {
			SDL_Log("Could not load font texture!");
			return;
		}

		std::string layout_path = "resources/font_layouts/" + font_name + ".csv";
		load_font_layout(layout_path);

		SDL_Log("%f", tex_coord[46][1]);
	}
};