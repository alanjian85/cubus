#ifndef CEPHALON_PLANE_H_
#define CEPHALON_PLANE_H_

#include <glm/glm.hpp>

namespace cephalon {
    class Plane {
    public:
        Plane() = default;

        Plane(glm::vec3 normal, float distance) {
            normal_ = normal;
            distance_ = distance;
        }
    private:
        glm::vec3 normal_;
        float distance_;
    };
}

#endif // CEPHALON_PLANE_H_