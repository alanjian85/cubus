#ifndef CEPHALON_AABB_H_
#define CEPHALON_AABB_H_

#include <optional>

#include <glm/glm.hpp>

namespace cephalon {
    enum class Direction {
        kRight,
        kLeft,
        kTop,
        kBottom,
        kBack,
        kFront
    };

    struct AABB {
        AABB() = default;
        AABB(glm::vec3 min_, glm::vec3 max_)
            : min(min_), max(max_)
        {

        }

        std::optional<Direction> intersect(glm::vec3 pos, glm::vec3 dir, float t_min, float t_max) const {
            for (int i = 0; i < 3; ++i) {
                auto invd = 1.0f / dir[i];
                auto t0 = (min[i] - pos[i]) * invd;
                auto t1 = (max[i] - pos[i]) * invd;
                if (invd < 0.0f)
                    std::swap(t0, t1);
                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;
                if (t_max <= t_min)
                    return std::nullopt;
            }
            
            auto point = pos + dir * t_min - (min + max) / 2.0f;
            if (std::abs(point.x) > std::abs(point.y) && std::abs(point.x) > std::abs(point.z)) {
                if (point.x > 0)
                    return Direction::kRight;
                else
                    return Direction::kLeft;
            }
            else if (std::abs(point.y) > std::abs(point.x) && std::abs(point.y) > std::abs(point.z)) {
                if (point.y > 0)
                    return Direction::kTop;
                else
                    return Direction::kBottom;
            }
            else {
                if (point.z > 0)
                    return Direction::kBack;
                else
                    return Direction::kFront;
            }
        }

        glm::vec3 min;
        glm::vec3 max;
    };
}

#endif // CEPHALON_AABB_H_