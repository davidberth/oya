module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

export module camera;

export class Camera {
public:
    glm::vec2 position;
    float height;
    float rotation;

    Camera(glm::vec2 start_pos, float start_height, float start_rotation)
        : position(start_pos), height(start_height), rotation(start_rotation) {}

    glm::mat4 getViewMatrix() const {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(-position, -height));
        //view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        

        return view;
    }
};

export Camera camera(glm::vec2(0.0f, 0.0f), 3.0f, 0.0f);
