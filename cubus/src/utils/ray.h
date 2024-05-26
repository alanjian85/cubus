#pragma once

#include <glm/glm.hpp>

namespace cephalon {
    struct Ray {
        Ray(glm::vec3 pos_, glm::vec3 dir_) {
            pos = pos_;
            dir = dir_;
        }

        glm::vec3 pos;
        glm::vec3 dir;
    };
}
