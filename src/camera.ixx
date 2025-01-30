module;

#include <glm/ext/matrix_projection.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <math.h>


#include <iostream>

export module camera;

import updatable;
import input_manager;
import input_event;
import viewport;
import mouse_event;

export class Camera : public Updatable {
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
	float pan_speed = 0.005f;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 view_proj;
	glm::mat4 view_proj_inv;


	bool is_mouse_down = false;
	glm::vec2 mouse_pos;


	Camera(glm::vec2 start_pos, float start_height, float start_rotation)
		: position(start_pos), height(start_height), rotation(start_rotation)
	{
		printf("Creating camera\n");
		get_event_dispatcher()->subscribe<InputEvent>([this](const InputEvent& input_event) {
			on_input(input_event);
			});

		get_event_dispatcher()->subscribe<MouseMoveEvent>([this](const MouseMoveEvent& mouse_event) {
			on_mouse_move(mouse_event);
			});

		get_event_dispatcher()->subscribe<MouseButtonEvent>([this](const MouseButtonEvent& mouse_event) {
			if (mouse_event.button == MouseButton::right && mouse_event.action == MouseAction::press)
			{
				is_mouse_down = true;
			}
			if (mouse_event.button == MouseButton::right && mouse_event.action == MouseAction::release)
			{
				is_mouse_down = false;
			}
			});


	}

	glm::vec2 ndc_to_world_at_z(glm::vec2 ndc_pos, float target_z)
	{

		glm::vec3 camera_pos = -glm::vec3(position.x, position.y, -height);
		float z_eye = camera_pos.z - target_z;
		float z_ndc = ((1.0f / z_eye) - (1.0f / near_clip)) / ((1.0f / far_clip) - (1.0f / near_clip)) * 2.0f - 1.0f;

		glm::vec4 clip_pos(ndc_pos.x, ndc_pos.y, z_ndc, 1.0f);
		glm::vec4 world_pos = view_proj_inv * clip_pos;
		world_pos /= world_pos.w;

		return glm::vec2(world_pos.x, world_pos.y);
	}

	void update() override {

		// update the matrices
		Viewport* viewport = get_viewport();
		float asp_ratio = float(viewport->width) / float(viewport->height);
		projection = glm::perspective(glm::radians(45.0f), asp_ratio, near_clip, far_clip);
		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(-position.x, -position.y, -height));
		view = glm::rotate(view, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		view_proj = projection * view;
		view_proj_inv = glm::inverse(view_proj);


		if (get_input_manager()->get_input_state(InputAction::zoom_in))
		{
			height -= zoom_speed;
			if (height < 1.0f) height = 1.0f;
		}
		if (get_input_manager()->get_input_state(InputAction::zoom_out))
		{
			height += zoom_speed;
			if (height > 50.f) height = 50.0f;
		}

		if (get_input_manager()->get_input_state(InputAction::up))
		{
			position.y += speed;
		}
		if (get_input_manager()->get_input_state(InputAction::down))
		{
			position.y -= speed;
		}
		if (get_input_manager()->get_input_state(InputAction::left))
		{
			position.x -= speed;
		}
		if (get_input_manager()->get_input_state(InputAction::right))
		{
			position.x += speed;
		}


	}

	void on_input(const InputEvent& input_event)
	{
		if (input_event.action == InputAction::scroll_in)
		{
			height -= zoom_speed * 5.0f;
			if (height < 1.0f) height = 1.0f;
		}
		if (input_event.action == InputAction::scroll_out)
		{
			height += zoom_speed * 5.0f;
			if (height > 50.f) height = 50.0f;
		}

	}

	void on_mouse_move(const MouseMoveEvent& mouse_event)
	{
		if (is_mouse_down)
		{
			glm::vec2 delta = glm::vec2(mouse_event.xpos, mouse_event.ypos) - mouse_pos;
			delta.x = -delta.x;
			position += delta * pan_speed;

		}
		mouse_pos = glm::vec2(mouse_event.xpos, mouse_event.ypos);

	}

};

Camera* camera;

export inline Camera* get_camera()
{
	return camera;
}

export inline void init_camera()
{
	camera = new Camera(glm::vec2(0.0f, 0.0f), 10.0f, 0.0f);
}

export inline void delete_camera()
{
	delete camera;
}
