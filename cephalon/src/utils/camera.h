#ifndef CEPHALON_UTILS_CAMERA_H_
#define CEPHALON_UTILS_CAMERA_H_

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cephalon {
    struct PerspectiveCamera {
        void update() {
            dir = glm::vec3(
                glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
                glm::sin(glm::radians(pitch)),
                glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
            );
            right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), dir));
            view = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));
            proj = glm::perspective(glm::radians(fov), aspect, near, far);
        }

        glm::vec3 pos { 0.0f, 0.0f, 0.0f };

        float yaw = 90.0f;
        float pitch = 0.0f;

        float fov = 45.0f;
        float aspect;
        float near = 0.1f;
        float far = 1000.0f;

        glm::vec3 dir { 0.0f };
        glm::vec3 right { 0.0f };
        glm::mat4 view;
        glm::mat4 proj;
    };
}

#endif // CEPHALON_UTILS_CAMERA_H_