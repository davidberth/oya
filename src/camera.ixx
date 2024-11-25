module;

#include <glm/glm.hpp>
#include <glm/ext/matrix_projection.hpp>
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
import mouse_data;

export class Camera  : public Updatable {
public:
    glm::vec2 position;
    float height;
    float rotation;

	glm::vec2 forward;
	glm::vec2 side;

	float speed = 0.03f;
	float rot_speed = 0.005f;
	float zoom_speed = 0.09f;

	float near_clip = 0.1f;
	float far_clip = 100.0f;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 view_proj;
	glm::mat4 view_proj_inv;

    Camera(glm::vec2 start_pos, float start_height, float start_rotation)
        : position(start_pos), height(start_height), rotation(start_rotation) 
	{
	}

	glm::vec2 ndc_to_world_at_z(glm::vec2 ndc_pos, float target_z)
	{
		// Get camera position in world space
		// The camera position is the negative of the translation part of the view matrix
		glm::vec3 camera_pos = -glm::vec3(position.x, position.y, -height);
			

		// distance from camera to target z-plane
		float z_eye = camera_pos.z - target_z;  

		// compute the NDC z for the target z-plane
		float z_ndc = ((1.0f / z_eye) - (1.0f / near_clip)) / ((1.0f / far_clip) - (1.0f / near_clip)) * 2.0f - 1.0f;

		// create clip space point
		glm::vec4 clip_pos(ndc_pos.x, ndc_pos.y, z_ndc, 1.0f);

		// transform to world space
		glm::vec4 world_pos = view_proj_inv * clip_pos;
		world_pos /= world_pos.w;

		return glm::vec2(world_pos.x, world_pos.y);
	}

	void update() override {

		// update the matrices
		float asp_ratio = float(viewport_data.width) / float(viewport_data.height);
		projection = glm::perspective(glm::radians(45.0f), asp_ratio, near_clip, far_clip);
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(-position.x, -position.y, -height));
		view = glm::rotate(view, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		view_proj = projection * view;
		view_proj_inv = glm::inverse(view_proj);


		// rotate the camera
		rotation += rot_speed;
        if (rotation > 6.283f) rotation -= 2 * 6.283f;
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

		if (mouse_scroll_data.yoffset > 0.0f)
		{
			height -= zoom_speed * 5.0;
			mouse_scroll_data.yoffset = 0.0f;
			if (height < 1.0f) height = 1.0f;
		}
		if (mouse_scroll_data.yoffset < 0.0f)
		{
			height += zoom_speed * 5.0;
			mouse_scroll_data.yoffset = 0.0f;
			if (height > 50.f) height = 50.0f;
		}
	}
};

export Camera camera(glm::vec2(0.0f, 0.0f), 8.0f, glm::radians(0.0f));
