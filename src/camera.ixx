module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <math.h>

#include <iostream>

export module camera;

import window_data;
import keyboard_data;
import updatable;
import viewport_data;

export class Camera  : public Updatable {
public:
    glm::vec2 position;
    float height;
    float rotation;

	glm::vec2 forward;
	glm::vec2 side;

	float speed = 0.03f;
	float rot_speed = 0.03f;
	float zoom_speed = 0.09f;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 view_proj;
	glm::mat4 view_proj_inv;

    Camera(glm::vec2 start_pos, float start_height, float start_rotation)
        : position(start_pos), height(start_height), rotation(start_rotation) 
	{
		
	}

	glm::vec2 ndc_to_world(glm::vec2 ndc_pos)
	{


		// convert NDC to homogeneous clip coordinates
		glm::vec4 clip_pos = glm::vec4(ndc_pos.x, ndc_pos.y, 0.0f, 1.0f);

	
		// convert clip coordinates to world coordinates
		glm::vec4 world_pos = view_proj_inv * clip_pos;

		// perform perspective divide
		// world_pos /= world_pos.w;

		return glm::vec2(world_pos.x, world_pos.y);
	}

	void update() override {

		// update the matrices
		float asp_ratio = float(viewport_data[0].width) / float(viewport_data[0].height);
		projection = glm::perspective(glm::radians(45.0f), asp_ratio, 0.1f, 100.0f);
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(-position.x, -position.y, -height));
		view = glm::rotate(view, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		view_proj = projection * view;
		view_proj_inv = glm::inverse(view_proj);


		if (keyboard_data.zoom_in_down)
		{
			height -= zoom_speed;
			if (height < 1.0f) height = 1.0f;
		}
		if (keyboard_data.zoom_out_down)
		{
			height += zoom_speed;
			if (height > 50.f) height = 50.0f;
		}
	}
};

export Camera camera(glm::vec2(0.0f, 0.0f), 8.0f, glm::radians(90.0f));
