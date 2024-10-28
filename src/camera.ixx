module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <math.h>

export module camera;

import window_data;
import keyboard_data;
import updatable;

export class Camera  : public Updatable {
public:
    glm::vec2 position;
    float height;
    float rotation;

	glm::vec2 forward;
	glm::vec2 side;

	float speed = 0.03f;
	float rot_speed = 0.03f;
	float zoom_speed = 0.03f;

    Camera(glm::vec2 start_pos, float start_height, float start_rotation)
        : position(start_pos), height(start_height), rotation(start_rotation) 
	{
		compute_forward_side_vectors();
	}

    glm::mat4 get_view_proj_matrix() const {
        float asp_ratio = float(window_data.width) / float(window_data.height);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), asp_ratio, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(-position.x, -position.y, -height));
        view = glm::rotate(view, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

        return projection * view;
    }

	void compute_forward_side_vectors()
	{
		forward = glm::vec2(cos(rotation), sin(rotation));
		side = glm::vec2(sin(rotation), -cos(rotation));
	}

	void update() override {
		if (keyboard_data.up_down) {
			position -= forward * speed;
		}
		if (keyboard_data.down_down) {
			position += forward * speed;
		}
		if (keyboard_data.left_down) {
			position -= side * speed;
		}
		if (keyboard_data.right_down) {
			position += side * speed;
		}
		if (keyboard_data.rotate_right_down) {
			rotation += rot_speed;
			compute_forward_side_vectors();
		}
		if (keyboard_data.rotate_left_down) {
			rotation -= rot_speed;
			compute_forward_side_vectors();
		}
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
