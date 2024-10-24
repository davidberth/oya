module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

export module camera;

import window_data;

export class Camera {
public:
    glm::vec2 position;
    float height;
    float rotation;

    Camera(glm::vec2 start_pos, float start_height, float start_rotation)
        : position(start_pos), height(start_height), rotation(start_rotation) {}

    glm::mat4 get_view_proj_matrix() const {
        float asp_ratio = float(window_data.width / window_data.height);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), asp_ratio, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(-position.x, -position.y, -height));
        view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        return projection * view;
    }
};

export Camera camera(glm::vec2(0.0f, 0.0f), 8.0f, 15.0f);
